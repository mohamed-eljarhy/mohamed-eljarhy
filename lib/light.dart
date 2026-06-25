// ignore_for_file: deprecated_member_use, prefer_const_constructors

import 'package:flutter/material.dart';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';
import 'package:audioplayers/audioplayers.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'package:vipcar_v2/main.dart'; // This line imports espIp and color

class LightPage extends StatefulWidget {
  const LightPage({super.key});

  @override
  State<LightPage> createState() => _LightPageState();
}

class _LightPageState extends State<LightPage> {
  double _volume = 1.0;
  final AudioPlayer _audioPlayer = AudioPlayer();

  @override
  void dispose() {
    _audioPlayer.dispose();
    super.dispose();
  }

  // Helper function to build the request URL
  String getUrl(String path) {
    return 'http://$espIp/$path';
  }

  void picker1(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    showDialog(
      context: context,
      builder: (context) {
        return AlertDialog(
          title: Text(isArabic ? "ضبط اللون" : "Set Color"),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              color1(),
              const SizedBox(height: 16),
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  backgroundColor: color,
                  foregroundColor: Colors.white,
                  minimumSize: const Size(double.infinity, 45),
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(12),
                  ),
                ),
                onPressed: () {
                  _audioPlayer.play(AssetSource('sounds/click.mp3'));
                  Navigator.pop(context);
                  sendColor(colorToHex(color), _volume);
                },
                child: Text(isArabic ? "تأكيد" : "Submit"),
              ),
            ],
          ),
        );
      },
    );
  }

  ColorPicker color1() => ColorPicker(
    pickerColor: color, // Uses the variable from main.dart
    onColorChanged: (value) => setState(() {
      color = value; // Updates global variable
      sendColor(colorToHex(color), _volume);
    }),
  );

  @override
  Widget build(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    return Scaffold(
      backgroundColor: const Color(0xFF7D5A3C), // Luxury dark background
      appBar: AppBar(
        title: Column(
          children: [
            Text(
              isArabic ? 'التحكم في الإضاءة' : 'Light Control',
              style: TextStyle(fontWeight: FontWeight.bold, fontSize: 18),
            ),
            // Text(
            //   espIp == null ? "IP not set" : "Connected to: $espIp",
            //   style: TextStyle(
            //     fontSize: 11,
            //     fontWeight: FontWeight.w500,
            //     color: espIp == null ? Colors.redAccent : Colors.greenAccent,
            //   ),
            // ),
          ],
        ),
        centerTitle: true,
        backgroundColor: const Color(0xFF1E1E1E),
        elevation: 0,
      ),
      body: SafeArea(
        child: Center(
          child: ConstrainedBox(
            constraints: const BoxConstraints(maxWidth: 600),
            child: ScrollConfiguration(
              behavior: ScrollConfiguration.of(
                context,
              ).copyWith(scrollbars: false),
              child: SingleChildScrollView(
                padding: const EdgeInsets.symmetric(
                  horizontal: 20.0,
                  vertical: 16.0,
                ),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.stretch,
                  children: [
                    // 1. ON/OFF Switches
                    Row(
                      children: [
                        Expanded(
                          child: _switchButton(
                            isArabic ? "تشغيل" : "Light ON",
                            lightOm,
                            true,
                          ),
                        ),
                        const SizedBox(width: 10),
                        Expanded(
                          child: _switchButton(
                            isArabic ? "إطفاء" : "Light OFF",
                            lightOff,
                            false,
                          ),
                        ),
                      ],
                    ),
                    const SizedBox(height: 20),

                    // 2. Brightness
                    _buildBrightnessCard(),
                    const SizedBox(height: 20),

                    // 3. Custom Color
                    _buildCustomColorPickerWidget(context),
                    const SizedBox(height: 20),

                    // 4. Quick Colors
                    _buildPresetsCard(),
                    const SizedBox(height: 20),

                    // 5. Spectrum effect (Gradient)
                    _actionBox(
                      isArabic
                          ? "ألوان الطيف (تدرج)"
                          : "Spectrum Clrs(Gradient)",
                      [
                        Colors.red,
                        Colors.yellow,
                        Colors.green,
                        Colors.blue,
                        Colors.purple,
                      ],
                      grad,
                    ),
                    const SizedBox(height: 24),
                  ],
                ),
              ),
            ),
          ),
        ),
      ),
    );
  }

  // --- Design Helper Functions ---

  Widget _buildBrightnessCard() {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    return Container(
      decoration: BoxDecoration(
        color: const Color(0xFF1E1E1E),
        borderRadius: BorderRadius.circular(20),
        border: Border.all(color: Colors.white.withOpacity(0.05)),
      ),
      padding: const EdgeInsets.all(16),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            children: [
              Text(
                "${(_volume * 100).toInt()}%",
                style: const TextStyle(
                  fontSize: 16,
                  fontWeight: FontWeight.bold,
                  color: Colors.white,
                ),
              ),
              Row(
                children: [
                  Text(
                    isArabic ? "السطوع" : "Brightness",
                    style: const TextStyle(
                      fontSize: 15,
                      fontWeight: FontWeight.w600,
                      color: Colors.white70,
                    ),
                  ),
                  SizedBox(width: 8),
                  Icon(Icons.wb_sunny_outlined, color: Colors.amber, size: 18),
                ],
              ),
            ],
          ),
          const SizedBox(height: 12),
          SliderTheme(
            data: SliderThemeData(
              activeTrackColor: color,
              inactiveTrackColor: color.withOpacity(0.2),
              thumbColor: color,
              overlayColor: color.withOpacity(0.1),
              trackHeight: 6,
            ),
            child: Slider(
              value: _volume,
              onChanged: (value) {
                setState(() {
                  _volume = value;
                  sendColor(colorToHex(color), _volume);
                });
              },
              min: 0.0,
              max: 1.0,
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildCustomColorPickerWidget(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    return Container(
      decoration: BoxDecoration(
        color: const Color(0xFF1E1E1E),
        borderRadius: BorderRadius.circular(20),
        border: Border.all(color: Colors.white.withOpacity(0.05)),
      ),
      padding: const EdgeInsets.all(16),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.start,
            children: [
              Text(
                isArabic ? "لون مخصص" : "Custom Color",
                style: const TextStyle(
                  fontSize: 15,
                  fontWeight: FontWeight.w600,
                  color: Colors.white70,
                ),
              ),
              SizedBox(width: 8),
              Icon(Icons.palette_outlined, color: Colors.blueAccent, size: 18),
            ],
          ),
          const SizedBox(height: 14),
          InkWell(
            onTap: () => picker1(context),
            borderRadius: BorderRadius.circular(16),
            child: Container(
              height: 80,
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(16),
                color: color,
                boxShadow: [
                  BoxShadow(
                    color: color.withOpacity(0.3),
                    blurRadius: 12,
                    spreadRadius: 1,
                  ),
                ],
                border: Border.all(
                  color: Colors.white.withOpacity(0.6),
                  width: 1.5,
                ),
              ),
              child: Center(
                child: Container(
                  padding: const EdgeInsets.symmetric(
                    horizontal: 16,
                    vertical: 8,
                  ),
                  decoration: BoxDecoration(
                    color: Colors.black54,
                    borderRadius: BorderRadius.circular(20),
                  ),
                  child: Row(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      Icon(Icons.colorize, color: Colors.white, size: 16),
                      SizedBox(width: 8),
                      Text(
                        isArabic
                            ? "اضغط هنا لاختيار لون"
                            : "Tap here to pick a color",
                        style: const TextStyle(
                          color: Colors.white,
                          fontWeight: FontWeight.bold,
                          fontSize: 10,
                        ),
                      ),
                    ],
                  ),
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildPresetsCard() {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    final presets = [
      const Color(0xFFFF3B30), // Red
      const Color(0xFFFF9500), // Orange
      const Color(0xFFFFCC00), // Yellow
      const Color(0xFF34C759), // Green
      const Color(0xFF00C7BE), // Teal
      const Color(0xFF32ADE6), // Cyan
      const Color(0xFF007AFF), // Blue
      const Color(0xFF5856D6), // Indigo
      const Color(0xFFAF52DE), // Purple
      const Color(0xFFFF2D55), // Pink
      const Color(0xFFFFFFFF), // Cool White
      const Color(0xFFFFFDD0), // Warm White
    ];

    return Container(
      decoration: BoxDecoration(
        color: const Color(0xFF1E1E1E),
        borderRadius: BorderRadius.circular(20),
        border: Border.all(color: Colors.white.withOpacity(0.05)),
      ),
      padding: const EdgeInsets.all(16),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: [
          Row(
            mainAxisAlignment: MainAxisAlignment.start,
            children: [
              Text(
                isArabic ? "ألوان سريعة" : "Quick Colors",
                style: const TextStyle(
                  fontSize: 15,
                  fontWeight: FontWeight.w600,
                  color: Colors.white70,
                ),
              ),
              SizedBox(width: 8),
              Icon(Icons.flash_on_outlined, color: Colors.amber, size: 18),
            ],
          ),
          const SizedBox(height: 16),
          Center(
            child: Wrap(
              spacing: 12,
              runSpacing: 12,
              children: presets.map((c) => _colorBox(c)).toList(),
            ),
          ),
        ],
      ),
    );
  }

  Widget _colorBox(Color boxColor) {
    bool isSelected = color.value == boxColor.value;
    return GestureDetector(
      onTap: () {
        _audioPlayer.play(AssetSource('sounds/click.mp3'));
        setState(() => color = boxColor);
        sendColor(colorToHex(color), _volume);
      },
      child: Container(
        width: 46,
        height: 46,
        decoration: BoxDecoration(
          color: boxColor,
          shape: BoxShape.circle,
          border: Border.all(
            color: isSelected ? Colors.white : Colors.white.withOpacity(0.2),
            width: isSelected ? 3.0 : 1.0,
          ),
          boxShadow: [
            BoxShadow(
              color: boxColor.withOpacity(0.3),
              blurRadius: isSelected ? 8 : 2,
              spreadRadius: isSelected ? 1 : 0,
            ),
          ],
        ),
        child: isSelected
            ? const Icon(Icons.check, color: Colors.white, size: 18)
            : null,
      ),
    );
  }

  Widget _actionBox(String text, List<Color> colors, Function action) {
    return Container(
      height: 65,
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(20),
        gradient: LinearGradient(
          colors: colors,
          begin: Alignment.topLeft,
          end: Alignment.bottomRight,
        ),
        boxShadow: [
          BoxShadow(
            color: colors.first.withOpacity(0.2),
            blurRadius: 8,
            offset: const Offset(0, 4),
          ),
        ],
      ),
      child: Material(
        color: Colors.transparent,
        child: InkWell(
          borderRadius: BorderRadius.circular(20),
          onTap: () => action(),
          child: Center(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Icon(
                  Icons.palette_outlined,
                  color: Colors.white,
                  size: 22,
                ),
                const SizedBox(width: 10),
                Text(
                  text,
                  style: const TextStyle(
                    fontSize: 16,
                    fontWeight: FontWeight.bold,
                    color: Colors.white,
                    shadows: [
                      Shadow(
                        blurRadius: 4,
                        color: Colors.black54,
                        offset: Offset(0, 1.5),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _switchButton(String label, Function action, bool isOn) {
    Color btnColor = isOn ? Colors.green : Colors.red;
    Color textThemeColor = isOn ? Colors.green.shade100 : Colors.red.shade100;
    return Container(
      height: 55,
      decoration: BoxDecoration(
        borderRadius: BorderRadius.circular(16),
        color: btnColor.withOpacity(0.08),
        border: Border.all(color: btnColor.withOpacity(0.3), width: 1.5),
      ),
      child: Material(
        color: Colors.transparent,
        child: InkWell(
          borderRadius: BorderRadius.circular(16),
          onTap: () => action(),
          child: Center(
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Icon(
                  isOn ? Icons.power_settings_new : Icons.power_off,
                  color: btnColor,
                  size: 20,
                ),
                const SizedBox(width: 8),
                Text(
                  label,
                  style: TextStyle(
                    fontSize: 15,
                    fontWeight: FontWeight.bold,
                    color: textThemeColor,
                  ),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  String colorToHex(Color color) {
    final hex = color.toARGB32().toRadixString(16).padLeft(8, '0').toUpperCase();
    return '#${hex.substring(2)}';
  }

  Future sendColor(String hexColor, double volume) async {
    if (espIp == null) return;
    try {
      await http.post(
        Uri.parse(getUrl('handlergbcolor')),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({'color': hexColor, '_volume': volume}),
      );
    } catch (_) {}
  }

  Future grad() async {
    await _audioPlayer.play(AssetSource('sounds/click.mp3'));
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('handlegrad')));
  }

  Future lightOm() async {
    await _audioPlayer.play(AssetSource('sounds/click.mp3'));
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('handlelighton')));
  }

  Future lightOff() async {
    await _audioPlayer.play(AssetSource('sounds/click.mp3'));
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('handlelightoff')));
  }

  Future deepLed() async {
    await _audioPlayer.play(AssetSource('sounds/click.mp3'));
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    if (espIp != null) await http.get(Uri.parse(getUrl('deepLight')));
  }
}
