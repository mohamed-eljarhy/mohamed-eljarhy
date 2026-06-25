// ignore_for_file: prefer_const_constructors, camel_case_types

import 'package:flutter/material.dart';

class tv_sm extends StatefulWidget {
  const tv_sm({super.key});

  @override
  State<tv_sm> createState() => _tv_smState();
}

class _tv_smState extends State<tv_sm> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
        backgroundColor: Color.fromARGB(251, 49, 58, 58),
        appBar: AppBar(
            title:
                Center(child: Text("TV_SM", style: TextStyle(fontSize: 20)))),
        body: Center(
            child: Column(children: [
          SizedBox(height: 20),
          Container(
              height: 100,
              width: 200,
              decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(30),
                  color: const Color.fromARGB(255, 57, 70, 72)),
              child: InkWell(onTap: () {}))
        ])));
  }
}
