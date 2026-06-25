// ignore_for_file: prefer_const_constructors, camel_case_types

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:audioplayers/audioplayers.dart';
import 'package:vipcar_v2/main.dart';

class spresso extends StatefulWidget {
  const spresso({super.key});

  @override
  State<spresso> createState() => _spressoState();
}

class _spressoState extends State<spresso> with TickerProviderStateMixin {
  late AnimationController _steamController;
  late AnimationController _scaleController;
  late Animation<double> _scaleAnimation;
  final AudioPlayer _audioPlayer = AudioPlayer(); // مشغل الصوت الخاص بالقهوة

  @override
  void initState() {
    super.initState();
    _steamController = AnimationController(
      vsync: this,
      duration: const Duration(seconds: 3),
    )..repeat();

    _scaleController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 100),
    );
    _scaleAnimation = Tween<double>(begin: 1.0, end: 0.92).animate(
      CurvedAnimation(parent: _scaleController, curve: Curves.easeInOut),
    );
  }

  @override
  void dispose() {
    _steamController.dispose();
    _scaleController.dispose();
    _audioPlayer.dispose(); // تنظيف الموارد
    super.dispose();
  }

  String getUrl(String endpoint) {
    return 'http://$espIp/$endpoint';
  }

  Future<void> _handleEspresso() async {
    try {
      // تشغيل صوت النقرة الموحد عند البدء
      _audioPlayer.play(AssetSource('sounds/click.mp3'));
    } catch (e) {
      debugPrint("Audio Error: $e");
    }

    if (espIp != null) {
      try {
        await http.get(Uri.parse(getUrl('handlespresso')));
      } catch (e) {
        debugPrint("Error sending espresso command: $e");
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    return Scaffold(
      backgroundColor: const Color(0xFF7D5A3C),
      appBar: AppBar(
        title: Center(
          child: Text(
            isArabic ? "التحكم بالقهوة" : "Caffe Control",
            style: TextStyle(fontSize: 20),
          ),
        ),
        backgroundColor: const Color(0xFF1E1E1E),
        elevation: 4,
      ),
      body: SingleChildScrollView(
        child: Center(
          child: Padding(
            padding: const EdgeInsets.all(24.0),
            child: Column(
              children: [
                ScaleTransition(
                  scale: _scaleAnimation,
                  child: Container(
                    width: double.infinity,
                    constraints: BoxConstraints(maxWidth: 500),
                    decoration: BoxDecoration(
                      color: const Color(0xFF7D5A3C),
                      borderRadius: BorderRadius.circular(35),
                      boxShadow: [
                        BoxShadow(
                          color: Colors.black.withValues(alpha: 0.3),
                          blurRadius: 25,
                          offset: const Offset(0, 15),
                        ),
                      ],
                    ),
                    child: Column(
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        Padding(
                          padding: const EdgeInsets.all(25.0),
                          child: Container(
                            width: double.infinity,
                            height: 280,
                            decoration: BoxDecoration(
                              color: Colors.white.withValues(alpha: 0.05),
                              borderRadius: BorderRadius.circular(25),
                              border: Border.all(color: Colors.white10),
                            ),
                            child: Stack(
                              alignment: Alignment.center,
                              children: [
                                Positioned(
                                  top: 40,
                                  child: Icon(
                                    Icons.coffee_maker,
                                    size: 140,
                                    color: Colors.brown.shade300,
                                  ),
                                ),
                                Positioned(
                                  bottom: 30,
                                  child: Container(
                                    height: 6,
                                    width: 80,
                                    decoration: BoxDecoration(
                                      color: Colors.brown.withValues(
                                        alpha: 0.5,
                                      ),
                                      borderRadius: BorderRadius.circular(10),
                                    ),
                                  ),
                                ),
                              ],
                            ),
                          ),
                        ),
                        InkWell(
                          onTapDown: (_) => _scaleController.forward(),
                          onTapUp: (_) => _scaleController.reverse(),
                          onTapCancel: () => _scaleController.reverse(),
                          onTap: _handleEspresso,
                          borderRadius: const BorderRadius.only(
                            bottomLeft: Radius.circular(35),
                            bottomRight: Radius.circular(35),
                          ),
                          child: Container(
                            width: double.infinity,
                            padding: const EdgeInsets.symmetric(vertical: 30),
                            decoration: BoxDecoration(
                              color: Colors.black.withValues(alpha: 0.15),
                              borderRadius: const BorderRadius.only(
                                bottomLeft: Radius.circular(35),
                                bottomRight: Radius.circular(35),
                              ),
                            ),
                            child: Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: [
                                const Icon(
                                  Icons.play_circle_fill,
                                  color: Colors.white,
                                  size: 30,
                                ),
                                const SizedBox(width: 15),
                                Text(
                                  isArabic ? "بدء التحضير" : "START ESPRESSO",
                                  style: const TextStyle(
                                    color: Colors.white,
                                    fontWeight: FontWeight.bold,
                                    fontSize: 22,
                                    letterSpacing: 1.2,
                                  ),
                                ),
                              ],
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                ),
                const SizedBox(height: 50),
                Stack(
                  alignment: Alignment.bottomCenter,
                  children: [
                    SizedBox(
                      height:
                          330, // Adjusted height for steam (original 220 * 1.5)
                      width: 330,
                      child: AnimatedBuilder(
                        animation: _steamController,
                        builder: (context, child) {
                          return Stack(
                            children: List.generate(3, (index) {
                              final double animationValue =
                                  (_steamController.value + (index * 0.33)) %
                                  1.0;
                              return Positioned(
                                bottom:
                                    210 + // Adjusted steam origin (original 140 * 1.5)
                                    (animationValue *
                                        120), // Adjusted steam travel (original 80 * 1.5)
                                left:
                                    120.0 + // Adjusted steam horizontal (original 80 * 1.5)
                                    (index * 30) - // (original 20 * 1.5)
                                    (animationValue *
                                        15), // Adjusted steam spread (original 10 * 1.5)
                                child: Opacity(
                                  opacity: (1 - animationValue).clamp(0.0, 0.5),
                                  child: Icon(
                                    Icons.cloud,
                                    color: const Color(
                                      0xFFF5F5F5,
                                    ).withValues(alpha: 0.4),
                                    size:
                                        30 +
                                        (animationValue *
                                            45), // (original 20 & 30 * 1.5)
                                  ),
                                ),
                              );
                            }),
                          );
                        },
                      ),
                    ),
                    Image.asset(
                      'assets/images/cup-caffee.png',
                      width: 330, // Size of the cup image (original 220 * 1.5)
                      height: 270, // Size of the cup image (original 180 * 1.5)
                      fit: BoxFit.contain,
                    ),
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
