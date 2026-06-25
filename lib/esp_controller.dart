import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

class EspController extends ChangeNotifier {
  // نمط Singleton للوصول للمتحكم من أي مكان
  static final EspController instance = EspController._internal();
  EspController._internal();

  String? _ip;
  Color _color = const Color.fromARGB(255, 165, 131, 131);

  String? get ip => _ip;
  Color get color => _color;

  Future<void> init() async {
    final prefs = await SharedPreferences.getInstance();
    _ip = prefs.getString('last_esp_ip');
    notifyListeners();
  }

  Future<bool> setIp(String newIp) async {
    // التحقق من صحة عنوان IP بشكل بسيط
    if (newIp.isEmpty || !newIp.contains('.')) return false;

    _ip = newIp;
    final prefs = await SharedPreferences.getInstance();
    await prefs.setString('last_esp_ip', newIp);
    notifyListeners();
    return true;
  }

  void updateColor(Color newColor) {
    _color = newColor;
    notifyListeners();
  }

  String _getUrl(String endpoint) => 'http://$_ip/$endpoint';

  String colorToHex(Color color) {
    return color.toARGB32().toRadixString(16).padLeft(8, '0').toUpperCase();
  }

  Future<void> sendColor(Color color, double volume) async {
    if (_ip == null) return;
    try {
      await http.post(
        Uri.parse(_getUrl('handlergbcolor')),
        headers: {'Content-Type': 'application/json'},
        body: jsonEncode({
          'color': colorToHex(color),
          '_volume': volume.toString(),
        }),
      );
    } catch (e) {
      debugPrint("ESP Error: $e");
    }
  }

  Future<void> getAction(String endpoint) async {
    if (_ip == null) return;
    try {
      await http.get(Uri.parse(_getUrl(endpoint)));
    } catch (e) {
      debugPrint("ESP Error: $e");
    }
  }
}
