// ignore_for_file: deprecated_member_use, prefer_const_constructors

import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'package:audioplayers/audioplayers.dart';
// Import main file to access color and espIp
import 'package:vipcar_v2/main.dart';

class TablePage extends StatefulWidget {
  const TablePage({super.key});

  @override
  State<TablePage> createState() => _TablePageState();
}

class _TablePageState extends State<TablePage> {
  final AudioPlayer _audioPlayer = AudioPlayer();

  @override
  void dispose() {
    _audioPlayer.dispose();
    super.dispose();
  }

  // Helper function to create the URL based on the current IP
  String getUrl(String endpoint) {
    return 'http://$espIp/$endpoint';
  }

  @override
  Widget build(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    // Calculate screen dimensions for responsiveness between mobile and tablet
    final size = MediaQuery.of(context).size;
    final bool isTablet = size.width > 600;
    const int crossAxisCount = 1; // Put each card in a separate row

    return Scaffold(
      backgroundColor: const Color(
        0xFFC19A6B,
      ), // Background color set to luxury camel
      appBar: AppBar(
        title: Column(
          children: [
            Text(
              isArabic ? 'التحكم بالطاولات' : 'Table Control',
              style: TextStyle(fontWeight: FontWeight.bold, fontSize: 18),
            ),
            Text(
              espIp == null
                  ? (isArabic ? "IP غير مضبوط" : "IP not set")
                  : (isArabic ? "متصل بـ: $espIp" : "Connected to: $espIp"),
              style: TextStyle(
                fontSize: 11,
                fontWeight: FontWeight.w500,
                color: espIp == null ? Colors.redAccent : Colors.greenAccent,
              ),
            ),
          ],
        ),
        centerTitle: true,
        backgroundColor: const Color(0xFF1E1E1E), // Consistent App Bar color
        elevation: 4,
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: Container(
          decoration: BoxDecoration(
            borderRadius: BorderRadius.circular(20),
            color: const Color(
              0xFFB08D60,
            ), // Slightly darker camel color behind cards
          ),
          child: GridView.count(
            crossAxisCount: crossAxisCount,
            childAspectRatio: isTablet
                ? 3.2
                : 1.8, // زيادة الارتفاع بتقليل النسبة
            padding: const EdgeInsets.all(20),
            mainAxisSpacing: 20,
            crossAxisSpacing: 20,
            children: [
              _buildTableCard(
                isArabic ? "ترابيزة 1" : "Table 1",
                Icons.table_restaurant,
                const Color.fromARGB(255, 8, 8, 8),
                table1,
                1,
              ),
              _buildTableCard(
                isArabic ? "ترابيزة 2" : "Table 2",
                Icons.table_restaurant,
                const Color(0XFF00FF00),
                table2,
                2,
              ),
              _buildTableCard(
                isArabic ? "ترابيزة 3" : "Table 3",
                Icons.table_restaurant,
                const Color(0XFF0000FF),
                table3,
                3,
              ),
              _buildTableCard(
                isArabic ? "ترابيزة 4" : "Table 4",
                Icons.table_restaurant,
                const Color(0XFF8B00FF),
                table4,
                4,
              ),
            ],
          ),
        ),
      ),
    );
  }

  // Widget to build the table card with animation effect and icons
  Widget _buildTableCard(
    String label,
    IconData icon,
    Color color,
    Function action,
    int tableIndex,
  ) {
    return _PressableEffect(
      onTap: () => action(),
      child: Container(
        decoration: BoxDecoration(
          color: const Color.fromARGB(
            255,
            61,
            52,
            52,
          ), // Dark camel color consistent with the background
          borderRadius: BorderRadius.circular(20),
          border: Border.all(color: Colors.white.withOpacity(0.1), width: 1),
          boxShadow: [
            BoxShadow(
              color: color.withOpacity(
                0.25,
              ), // Colored shadow giving a light glow effect
              blurRadius: 20,
              offset: const Offset(0, 10),
            ),
            BoxShadow(
              color: Colors.black.withOpacity(
                0.05,
              ), // Soft black shadow for depth
              blurRadius: 10,
              offset: const Offset(0, 4),
            ),
          ],
        ),
        child: Row(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Padding(
              padding: const EdgeInsets.only(left: 30),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Icon(icon, color: color, size: 50),
                  const SizedBox(height: 12),
                  Text(
                    label,
                    style: const TextStyle(
                      color: Colors.white,
                      fontWeight: FontWeight.bold,
                      fontSize: 26,
                    ),
                  ),
                ],
              ),
            ),
            Container(
              margin: const EdgeInsets.only(right: 25),
              width: 110,
              height: 110,
              decoration: BoxDecoration(
                color: Colors.white.withOpacity(0.05),
                borderRadius: BorderRadius.circular(15),
                border: Border.all(color: Colors.white10),
              ),
              padding: const EdgeInsets.all(10),
              child: Column(
                children: [
                  Expanded(
                    child: Row(
                      children: [
                        _buildMapQuadrant(
                          tableIndex == 1,
                          color,
                        ), // Top-left quadrant
                        _buildMapQuadrant(
                          tableIndex == 4,
                          color,
                        ), // Top-right quadrant
                      ],
                    ),
                  ),
                  Expanded(
                    child: Row(
                      children: [
                        _buildMapQuadrant(
                          tableIndex == 2,
                          color,
                        ), // Bottom-left quadrant
                        _buildMapQuadrant(
                          tableIndex == 3,
                          color,
                        ), // Bottom-right quadrant
                      ],
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

  // Helper widget to draw the small square inside the virtual map
  Widget _buildMapQuadrant(bool isActive, Color color) {
    return Expanded(
      child: Container(
        margin: const EdgeInsets.all(4),
        decoration: BoxDecoration(
          color: isActive ? color : Colors.white.withOpacity(0.05),
          borderRadius: BorderRadius.circular(4),
        ),
        child: Center(
          child: Icon(
            Icons.table_restaurant,
            size: 14,
            color: isActive ? Colors.white : Colors.white.withOpacity(0.1),
          ),
        ),
      ),
    );
  }

  Future table1() async {
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('handletable1')));
  }

  Future table2() async {
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('handletable2')));
  }

  Future table3() async {
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('handletable3')));
  }

  Future table4() async {
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('handletable4')));
  }
}

// Helper class to add a Scale Effect
class _PressableEffect extends StatefulWidget {
  final Widget child;
  final VoidCallback onTap;

  const _PressableEffect({required this.child, required this.onTap});

  @override
  State<_PressableEffect> createState() => _PressableEffectState();
}

class _PressableEffectState extends State<_PressableEffect> {
  bool _isPressed = false;

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTapDown: (_) => setState(() => _isPressed = true),
      onTapUp: (_) => setState(() => _isPressed = false),
      onTapCancel: () => setState(() => _isPressed = false),
      onTap: widget.onTap,
      child: AnimatedScale(
        scale: _isPressed ? 0.94 : 1.0,
        duration: const Duration(milliseconds: 100),
        child: widget.child,
      ),
    );
  }
}
