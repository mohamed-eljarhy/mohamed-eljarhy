// ignore_for_file: prefer_const_constructors, use_build_context_synchronously, deprecated_member_use

import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/material.dart';
import 'package:flutter_localizations/flutter_localizations.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:audioplayers/audioplayers.dart';
import 'package:vipcar_v2/firebase_options.dart';
import 'package:vipcar_v2/light.dart';
import 'package:vipcar_v2/shutter.dart';
import 'package:vipcar_v2/spresso.dart';
import 'package:vipcar_v2/tables.dart';
//import 'package:vipcar_v2/test.dart';
//import 'package:vipcar_v2/tv_sm.dart';

// --- Global variables for access from any page ---
String? espIp;
Color color = Color.fromARGB(
  255,
  165,
  131,
  131,
); // Added global color definition

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp(options: DefaultFirebaseOptions.currentPlatform);

  final prefs = await SharedPreferences.getInstance();
  espIp = prefs.getString('last_esp_ip');

  runApp(const VIPCarApp());
}

class VIPCarApp extends StatefulWidget {
  const VIPCarApp({super.key});

  static void setLocale(BuildContext context, Locale newLocale) {
    _VIPCarAppState? state = context.findAncestorStateOfType<_VIPCarAppState>();
    state?.setLocale(newLocale);
  }

  @override
  State<VIPCarApp> createState() => _VIPCarAppState();
}

class _VIPCarAppState extends State<VIPCarApp> {
  Locale _locale = const Locale('en');

  void setLocale(Locale locale) {
    setState(() {
      _locale = locale;
    });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      theme: ThemeData.dark(),
      debugShowCheckedModeBanner: false,
      title: "VIP Car Control",
      locale: _locale,
      supportedLocales: const [Locale('en', ''), Locale('ar', '')],
      localizationsDelegates: const [
        GlobalMaterialLocalizations.delegate,
        GlobalWidgetsLocalizations.delegate,
        GlobalCupertinoLocalizations.delegate,
      ],
      builder: (context, child) {
        return Directionality(
          textDirection: TextDirection.ltr,
          child: child!,
        );
      },
      home: const Myapp(),
    );
  }
}

class Myapp extends StatefulWidget {
  const Myapp({super.key});

  @override
  State<Myapp> createState() => _MyappState();
}

class _MyappState extends State<Myapp> {
  final TextEditingController _ipController = TextEditingController();
  final AudioPlayer _audioPlayer = AudioPlayer();

  @override
  void dispose() {
    _ipController.dispose();
    _audioPlayer.dispose();
    super.dispose();
  }

  Future<void> _saveAndConnect(String ip) async {
    _audioPlayer.play(AssetSource('sounds/click.mp3'));
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('last_esp_ip', ip);
    setState(() {
      espIp = ip;
    });
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    ScaffoldMessenger.of(context).showSnackBar(
      SnackBar(content: Text(isArabic ? "تم حفظ الـ IP: $ip" : "IP Saved: $ip"), backgroundColor: Colors.green),
    );
  }

  void _showIpDialog() {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(isArabic ? "إعدادات الاتصال" : "Connection Settings"),
        content: TextField(
          controller: _ipController,
          decoration: InputDecoration(hintText: isArabic ? "أدخل عنوان IP لـ system" : "Enter system IP Address"),
          keyboardType: TextInputType.text,
        ),
        actions: [
          TextButton(
            onPressed: () {
              if (_ipController.text.isNotEmpty) {
                _saveAndConnect(_ipController.text);
                Navigator.pop(context);
              }
            },
            child: Text(isArabic ? "حفظ واتصال" : "Save & Connect"),
          ),
        ],
      ),
    );
  }

  Widget _buildLanguageCard(
    BuildContext context,
    String title,
    Locale locale,
    Color activeColor,
  ) {
    bool isSelected =
        Localizations.localeOf(context).languageCode == locale.languageCode;
    return GestureDetector(
      onTap: () {
        _audioPlayer.play(AssetSource('sounds/click.mp3'));
        VIPCarApp.setLocale(context, locale);
      },
      child: Container(
        height: 80,
        decoration: BoxDecoration(
          color: const Color(0xFF1E1E1E),
          borderRadius: BorderRadius.circular(20),
          border: Border.all(
            color: isSelected ? activeColor : Colors.white10,
            width: 2,
          ),
          boxShadow: isSelected
              ? [
                  BoxShadow(
                    color: activeColor.withOpacity(0.3),
                    blurRadius: 10,
                    spreadRadius: 1,
                  ),
                ]
              : [],
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Icon(
              locale.languageCode == 'ar' ? Icons.translate : Icons.language,
              color: isSelected ? activeColor : Colors.white60,
              size: 28,
            ),
            const SizedBox(height: 8),
            Text(
              title,
              style: TextStyle(
                color: isSelected ? Colors.white : Colors.white60,
                fontWeight: isSelected ? FontWeight.bold : FontWeight.normal,
              ),
            ),
          ],
        ),
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    bool isArabic = Localizations.localeOf(context).languageCode == 'ar';
    final size = MediaQuery.of(context).size;
    double w = size.width;
    double h = size.height;

    return Scaffold(
      backgroundColor: const Color(0xFF7D5A3C), // Camel background color
      appBar: AppBar(
        title: Center(child: Text(isArabic ? " VIP Car" : " VIP Car", style: TextStyle(fontSize: 20))),
        actions: [
          IconButton(
            icon: Icon(
              Icons.settings_ethernet,
              color: espIp == null ? Colors.red : Colors.green,
            ),
            onPressed: _showIpDialog,
          ),
        ],
      ),
      body: Center(
        child: ListView(
          children: [
            Card(
              color: const Color.fromARGB(255, 52, 54, 50),
              elevation: 6,
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(16),
              ),
              clipBehavior: Clip.antiAlias,
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: [
                  Image.asset(
                    'assets/images/images.jpg',
                    height: 300,
                    width: double.infinity,
                    fit: BoxFit.cover,
                  ),
                ],
              ),
            ),

            if (espIp == null)
              Padding(
                padding: const EdgeInsets.all(8.0),
                child: Text(
                  isArabic ? "يرجى ضبط عنوان IP من أيقونة الإعدادات أعلاه" : "Please set the IP address from the settings icon above",
                  textAlign: TextAlign.center,
                  style: TextStyle(
                    color: Colors.red,
                    fontWeight: FontWeight.bold,
                  ),
                ),
              ),

            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 10),
              child: Row(
                children: [
                  Expanded(
                    child: _buildLanguageCard(
                      context,
                      "English",
                      const Locale('en'),
                      Colors.blueAccent,
                    ),
                  ),
                  const SizedBox(width: 15),
                  Expanded(
                    child: _buildLanguageCard(
                      context,
                      "العربية",
                      const Locale('ar'),
                      Colors.greenAccent,
                    ),
                  ),
                ],
              ),
            ),

            SizedBox(height: h * 0.03),

            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                _buildMenuButton(
                  context,
                  isArabic ? "الإضاءة" : "Control Light",
                  Icons.lightbulb_outline,
                  h,
                  w,
                  const LightPage(),
                ),
                _buildMenuButton(
                  context,
                  isArabic ? "الستائر" : "Shutter",
                  Icons.window_outlined,
                  h,
                  w,
                  shutter(),
                ),
              ],
            ),

            SizedBox(height: h * 0.03),

            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                _buildMenuButton(
                  context,
                  isArabic ? "قهوة" : "Spresso",
                  Icons.coffee_outlined,
                  h,
                  w,
                  spresso(),
                ),
                _buildMenuButton(
                  context,
                  isArabic ? "الترابيزات" : "Tables",
                  Icons.table_restaurant_outlined,
                  h,
                  w,
                  const TablePage(),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }

  Widget _buildMenuButton(
    BuildContext context,
    String title,
    IconData icon,
    double h,
    double w,
    Widget page,
  ) {
    return GestureDetector(
      onTap: () {
        _audioPlayer.play(AssetSource('sounds/click.mp3'));
        Navigator.push(context, MaterialPageRoute(builder: (context) => page));
      },
      child: Container(
        height: h * 0.20,
        width: w * 0.45,
        decoration: BoxDecoration(
          color: const Color(0xFF1E1E1E),
          borderRadius: BorderRadius.circular(25),
          border: Border.all(color: Colors.white10),
          boxShadow: [
            BoxShadow(
              color: Colors.indigo.withOpacity(0.5),
              blurRadius: 20,
              spreadRadius: 2,
              offset: const Offset(0, 8),
            ),
          ],
        ),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Container(
              padding: const EdgeInsets.all(16),
              decoration: BoxDecoration(
                color: Colors.white.withOpacity(0.05),
                shape: BoxShape.circle,
              ),
              child: Icon(icon, color: Colors.amberAccent, size: 48),
            ),
            const SizedBox(height: 15),
            Text(
              title,
              style: const TextStyle(
                fontWeight: FontWeight.bold,
                color: Colors.white,
                fontSize: 18,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
// ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAIGpo9GxN9VnQWJ4Q72MzTdd4RWGk53XD2nn2+yZu7WAm mohamedeljarhy@gmail.com