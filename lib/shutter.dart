// ignore_for_file: prefer_const_constructors, camel_case_types, deprecated_member_use

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:audioplayers/audioplayers.dart';
// Import espIp from main for dynamic connection
import 'package:vipcar_v2/main.dart';

class shutter extends StatefulWidget {
  const shutter({super.key});

  @override
  State<shutter> createState() => _shutterState();
}

class _shutterState extends State<shutter> {
  @override
  Widget build(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    return Scaffold(
      backgroundColor: const Color(0xFF7D5A3C),
      appBar: AppBar(
        title: Column(
          children: [
            Text(
              isArabic ? "التحكم بالستائر" : "Shutter Control",
              style: TextStyle(fontSize: 18),
            ),
            Text(
              espIp == null
                  ? (isArabic ? "IP غير مضبوط" : "IP not set")
                  : (isArabic ? "متصل بـ: $espIp" : "Connected to: $espIp"),
              style: TextStyle(
                fontSize: 12,
                color: espIp == null ? Colors.red : Colors.greenAccent,
              ),
            ),
          ],
        ),
        centerTitle: true,
      ),
      body: Center(
        child: ListView(
          children: [for (int i = 1; i <= 5; i++) ShutterRow(number: i)],
        ),
      ),
    );
  }
}

class ShutterRow extends StatefulWidget {
  final int number;
  const ShutterRow({super.key, required this.number});

  @override
  State<ShutterRow> createState() => _ShutterRowState();
}

class _ShutterRowState extends State<ShutterRow> with TickerProviderStateMixin {
  late AnimationController _controller;
  late Animation<double> _heightAnimation;
  final AudioPlayer _audioPlayer = AudioPlayer(); // مشغل الصوت الخاص بالستائر

  @override
  void initState() {
    super.initState();
    _controller = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 4000),
    );
    // begin: 185 (Closed/Down), end: 15 (Open/Up)
    _heightAnimation = Tween<double>(
      begin: 185.0,
      end: 15.0,
    ).animate(CurvedAnimation(parent: _controller, curve: Curves.easeInOut));
  }

  @override
  void dispose() {
    _controller.dispose();
    _audioPlayer.dispose();
    super.dispose();
  }

  String getUrl(String path) => 'http://$espIp/$path';

  Future<void> _sendAction(String endpoint) async {
    try {
      // تحديد الصوت بناءً على نوع الحركة (فتح/إغلاق أو توقف)
      if (endpoint.contains('shr') || endpoint.contains('shl')) {
        // استخدام صوت النقرة الموحد
        _audioPlayer.play(AssetSource('sounds/click.mp3'));
      } else if (endpoint.contains('shs')) {
        // صوت ضغطة زر التوقف
        _audioPlayer.play(AssetSource('sounds/click.mp3'));
      }
    } catch (e) {
      debugPrint("Audio Error: $e");
    }

    if (espIp != null) {
      final String fullCommand = 'handle$endpoint';
      try {
        await http.get(Uri.parse(getUrl(fullCommand)));
      } catch (e) {
        debugPrint("Error sending command $fullCommand: $e");
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    return Container(
      margin: const EdgeInsets.symmetric(horizontal: 16, vertical: 10),
      padding: const EdgeInsets.all(16),
      decoration: BoxDecoration(
        color: const Color(0xFF1E1E1E),
        borderRadius: BorderRadius.circular(20),
        border: Border.all(color: Colors.white10),
        boxShadow: [
          BoxShadow(
            color: const Color.fromARGB(
              255,
              113,
              129,
              236,
            ).withOpacity(0.6), // زيادة كثافة اللون لجعله أوضح
            blurRadius: 30, // زيادة التمويه لجعل الظل أكثر نعومة وانتشارًا
            spreadRadius: 3, // زيادة انتشار الظل قليلاً
            offset: const Offset(0, 9), // زيادة الإزاحة لجعله أكثر بروزًا
          ),
        ],
      ),
      child: IntrinsicHeight(
        child: Row(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            Expanded(
              flex: 3,
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    isArabic
                        ? "ستارة ${widget.number}"
                        : "Shutter ${widget.number}",
                    style: const TextStyle(
                      color: Colors.white,
                      fontSize: 18,
                      fontWeight: FontWeight.bold,
                    ),
                  ),
                  const SizedBox(height: 15),
                  _buildActionButton(
                    isArabic ? "فتح" : "OPEN",
                    (widget.number == 2 || widget.number == 3)
                        ? "shl${widget.number}"
                        : "shr${widget.number}",
                    onTap: () => _controller.forward(),
                  ),
                  const SizedBox(height: 8),
                  _buildActionButton(
                    isArabic ? "إيقاف" : "STOP",
                    "shs${widget.number}",
                    onTap: () => _controller.stop(),
                  ),
                  const SizedBox(height: 8),
                  _buildActionButton(
                    isArabic ? "إغلاق" : "CLOSE",
                    (widget.number == 2 || widget.number == 3)
                        ? "shr${widget.number}"
                        : "shl${widget.number}",
                    onTap: () => _controller.reverse(),
                  ),
                ],
              ),
            ),
            const SizedBox(width: 20),
            Expanded(
              flex: 4,
              child: Column(
                mainAxisAlignment: MainAxisAlignment.end,
                crossAxisAlignment: CrossAxisAlignment.end,
                children: [
                  _buildLocationIndicator(widget.number),
                  const SizedBox(height: 8),
                  AnimatedBuilder(
                    animation: _controller,
                    builder: (context, child) => _buildShutterVisualCard(
                      _heightAnimation.value,
                      _controller.value,
                    ),
                  ),
                ],
              ),
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildLocationIndicator(int number) {
    int totalWindows = (number <= 3) ? 3 : 2;
    return Container(
      width: 180, // مطابقة عرض مؤشر الموقع مع عرض الستارة
      padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 6),
      decoration: BoxDecoration(
        color: Colors.black38,
        borderRadius: BorderRadius.circular(8),
        border: Border.all(color: Colors.white10),
      ),
      child: Row(
        children: List.generate(totalWindows, (index) {
          // تحديد النافذة النشطة بناءً على الترتيب من اليمين إلى اليسار
          // لـ 3 نوافذ: اليمين (1) هي الاندكس الأخير، اليسار (3) هي الاندكس 0
          // لـ 2 نافذة: اليمين (4) هي الاندكس 1، اليسار (5) هي الاندكس 0
          bool isActive = (number <= 3)
              ? (index == (3 - number))
              : (index == (5 - number));

          return Expanded(
            child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 2),
              height: 25, // زيادة الارتفاع قليلاً ليصبح الشكل متناسقاً
              decoration: BoxDecoration(
                color: isActive ? Colors.amberAccent : Colors.white12,
                borderRadius: BorderRadius.circular(4),
                border: Border.all(
                  color: isActive ? Colors.amber : Colors.white24,
                ),
              ),
            ),
          );
        }),
      ),
    );
  }

  Widget _buildActionButton(
    String label,
    String endpoint, {
    VoidCallback? onTap,
  }) {
    return AnimatedContainer(
      duration: const Duration(milliseconds: 200),
      height: 60,
      width: double.infinity,
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(30),
        color: const Color.fromARGB(255, 82, 105, 96),
      ),
      child: InkWell(
        onTap: () {
          _sendAction(endpoint);
          if (onTap != null) onTap();
        },
        child: Center(
          child: Text(
            label,
            style: const TextStyle(
              color: Colors.white,
              fontWeight: FontWeight.bold,
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildShutterVisualCard(double currentHeight, double animationValue) {
    return Container(
      // This is the container for the shutter visual
      width: 180, // من هنا تتحكم في عرض شكل الستارة المتحركة
      height:
          170, // Fixed height to accommodate max shutter height (185) + padding
      decoration: BoxDecoration(
        color: Colors.black26,
        borderRadius: BorderRadius.circular(15),
        border: Border.all(color: Colors.white.withOpacity(0.05)),
      ),
      child: Stack(
        children: [
          Container(
            height: 4,
            width: double.infinity,
            decoration: BoxDecoration(
              color: Colors.grey[800],
              borderRadius: BorderRadius.circular(2),
            ),
          ),
          Align(
            alignment: Alignment.topCenter,
            child: Container(
              width: double.infinity,
              height: currentHeight,
              margin: const EdgeInsets.symmetric(horizontal: 12),
              decoration: BoxDecoration(
                color: Color.lerp(
                  const Color.fromARGB(255, 185, 126, 126),
                  const Color.fromARGB(255, 172, 130, 6).withOpacity(0.3),
                  _controller.value,
                ),
                borderRadius: const BorderRadius.vertical(
                  bottom: Radius.circular(5),
                ),
              ),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: List.generate(
                  8,
                  (index) => Container(
                    height: 1.5,
                    width: double.infinity,
                    color: Colors.black26,
                  ),
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
