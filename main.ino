#include <BlynkGOv3.h>

// ================= Global =================
GLabel label_0, label_1, label_2;
GButton btn_0, btn_1;

// Home
GRect rect_Home;
GButton btn_Display, btn_Setting;

// Setting (2x2 buttons)
GRect rect_Page1;
GButton btn_back_p1;
GLabel label_Page1;
GButton btn_ECG, btn_SpO2, btn_RESP, btn_NIBP;

// Button Page (แสดงชื่อปุ่ม)
GRect rect_ButtonPage;
GLabel label_ButtonPage;
GButton btn_back_BP;

// Display (Page2)
GRect rect_Page2;
GRect rect_Left, rect_Right;
GRect graph_bg_ecg, graph_bg_resp, graph_bg_spo2;
GLabel label_ecg_graph, label_resp_graph, label_spo2_graph;

GRect graph_resp_bg, graph_temp_bg, graph_spo2_bg, graph_bp_bg;
GLabel label_pr_title, label_temp_title, label_spo2_title, label_bp_title;
GButton btn_pr_value, btn_temp_value, btn_spo2_value, btn_sys_value, btn_dia_value;
GLabel label_time, label_date;
GButton btn_back_p2;

// ================= Variables =================
static float temp = 36.0;
static int pr = 60;
static int spo2 = 98;
static int sys = 100;
static int dia = 69;

std::vector<int> ecg_data, resp_data, spo2_data;
const int MAX_DATA_POINTS = 50;

// ================= Setup =================
void setup() {
  Serial.begin(9600);
  BlynkGO.begin();

  // ---------- Home ----------
  rect_Home.size(480, 320);
  rect_Home.color(TFT_WHITE);
  rect_Home.border(20, TFT_GRAY);
  rect_Home.corner_radius(20);
  rect_Home.hidden(false);

  label_0.parent(rect_Home);
  label_0 = "Vital Sign Monitor";
  label_0.color(TFT_BLACK);
  label_0.font(prasanmit_40);
  label_0.align(ALIGN_TOP, 0, 80);

  btn_Display.parent(rect_Home);
  btn_Display.size(100, 50);
  btn_Display = "Display";
  btn_Display.align(ALIGN_TOP, 75, 150);
  btn_Display.onClicked([](GWidget* widget) {
    rect_Home.hidden(true);
    rect_Page2.hidden(false);  // ไปหน้า Display จริง
  });

  btn_Setting.parent(rect_Home);
  btn_Setting.size(100, 50);
  btn_Setting = "Setting";
  btn_Setting.align(ALIGN_TOP, -75, 150);
  btn_Setting.onClicked([](GWidget* widget) {
    rect_Home.hidden(true);
    rect_Page1.hidden(false);
  });

  // ---------- Setting Page ----------
  rect_Page1.size(480, 320);
  rect_Page1.color(TFT_PURPLE);
  rect_Page1.border(50, TFT_GRAY);
  rect_Page1.corner_radius(20);
  rect_Page1.hidden(true);

  label_1.parent(rect_Page1);
  label_1 = "Setting";
  label_1.color(TFT_WHITE);
  label_1.font(prasanmit_40);
  label_1.align(ALIGN_TOP, 0, 5);

  btn_back_p1.parent(rect_Page1);
  btn_back_p1 = SYMBOL_ARROW_LEFT;
  btn_back_p1.round_design();
  btn_back_p1.align(ALIGN_TOP_LEFT, 3, 3);
  btn_back_p1.onClicked([](GWidget* widget){
    rect_Page1.hidden(true);
    rect_Home.hidden(false);
  });

  int btn_w = 150, btn_h = 80;
  int gap_x = 20, gap_y = 20;

  btn_ECG.parent(rect_Page1);
  btn_ECG.size(btn_w, btn_h);
  btn_ECG = "ECG";
  btn_ECG.color(TFT_WHITE);
  btn_ECG.align(ALIGN_CENTER, -btn_w/2 - gap_x/2, -btn_h/2 - gap_y/2);

  btn_SpO2.parent(rect_Page1);
  btn_SpO2.size(btn_w, btn_h);
  btn_SpO2 = "SpO2";
  btn_SpO2.color(TFT_WHITE);
  btn_SpO2.align(ALIGN_CENTER, btn_w/2 + gap_x/2, -btn_h/2 - gap_y/2);

  btn_RESP.parent(rect_Page1);
  btn_RESP.size(btn_w, btn_h);
  btn_RESP = "RESP";
  btn_RESP.color(TFT_WHITE);
  btn_RESP.align(ALIGN_CENTER, -btn_w/2 - gap_x/2, btn_h/2 + gap_y/2);

  btn_NIBP.parent(rect_Page1);
  btn_NIBP.size(btn_w, btn_h);
  btn_NIBP = "NIBP";
  btn_NIBP.color(TFT_WHITE);
  btn_NIBP.align(ALIGN_CENTER, btn_w/2 + gap_x/2, btn_h/2 + gap_y/2);

  // ---------- Button Detail Page ----------
  rect_ButtonPage.size(480, 320);
  rect_ButtonPage.color(TFT_PURPLE);
  rect_ButtonPage.border(50, TFT_GRAY);
  rect_ButtonPage.corner_radius(20);
  rect_ButtonPage.hidden(true);

  label_ButtonPage.parent(rect_ButtonPage);
  label_ButtonPage = "";
  label_ButtonPage.color(TFT_WHITE);
  label_ButtonPage.font(prasanmit_40);
  label_ButtonPage.align(ALIGN_TOP, 0, 5);

  btn_back_BP.parent(rect_ButtonPage);
  btn_back_BP = SYMBOL_ARROW_LEFT;
  btn_back_BP.round_design();
  btn_back_BP.align(ALIGN_TOP_LEFT, 3, 3);
  btn_back_BP.onClicked([](GWidget* widget){
    rect_ButtonPage.hidden(true);
    rect_Page1.hidden(false);
  });

  btn_ECG.onClicked([](GWidget* widget){
    rect_Page1.hidden(true);
    rect_ButtonPage.hidden(false);
    label_ButtonPage = "ECG";
  });
  btn_SpO2.onClicked([](GWidget* widget){
    rect_Page1.hidden(true);
    rect_ButtonPage.hidden(false);
    label_ButtonPage = "SpO2";
  });
  btn_RESP.onClicked([](GWidget* widget){
    rect_Page1.hidden(true);
    rect_ButtonPage.hidden(false);
    label_ButtonPage = "RESP";
  });
  btn_NIBP.onClicked([](GWidget* widget){
    rect_Page1.hidden(true);
    rect_ButtonPage.hidden(false);
    label_ButtonPage = "NIBP";
  });

  // ---------- Display Page (Page2) ----------
  rect_Page2.size(480, 320);
  rect_Page2.color(TFT_YELLOW);
  rect_Page2.border(5, TFT_GRAY);
  rect_Page2.hidden(true);

  // Panels
  rect_Right.parent(rect_Page2);
  rect_Right.size(150, 320);
  rect_Right.color(TFT_WHITE);
  rect_Right.border(3, TFT_BLACK);
  rect_Right.corner_radius(20);
  rect_Right.align(ALIGN_RIGHT);

  rect_Left.parent(rect_Page2);
  rect_Left.size(330, 320);
  rect_Left.color(TFT_WHITE);
  rect_Left.border(3, TFT_BLACK);
  rect_Left.corner_radius(20);
  rect_Left.align(ALIGN_LEFT);

  // Graphs
  graph_bg_ecg.parent(rect_Left);
  graph_bg_ecg.size(300, 80);
  graph_bg_ecg.color(TFT_WHITE);
  graph_bg_ecg.border(2, TFT_BLACK);
  graph_bg_ecg.align(ALIGN_TOP_MID, 0, 20);
  label_ecg_graph.parent(graph_bg_ecg);
  label_ecg_graph = "ECG";
  label_ecg_graph.color(TFT_BLACK);
  label_ecg_graph.font(prasanmit_20);
  label_ecg_graph.align(ALIGN_CENTER);

  graph_bg_resp.parent(rect_Left);
  graph_bg_resp.size(300, 80);
  graph_bg_resp.color(TFT_WHITE);
  graph_bg_resp.border(2, TFT_BLACK);
  graph_bg_resp.align(graph_bg_ecg, ALIGN_OUT_BOTTOM_MID, 0, 20);
  label_resp_graph.parent(graph_bg_resp);
  label_resp_graph = "Resp";
  label_resp_graph.color(TFT_BLACK);
  label_resp_graph.font(prasanmit_20);
  label_resp_graph.align(ALIGN_CENTER);

  graph_bg_spo2.parent(rect_Left);
  graph_bg_spo2.size(300, 80);
  graph_bg_spo2.color(TFT_WHITE);
  graph_bg_spo2.border(2, TFT_BLACK);
  graph_bg_spo2.align(graph_bg_resp, ALIGN_OUT_BOTTOM_MID, 0, 20);
  label_spo2_graph.parent(graph_bg_spo2);
  label_spo2_graph = "SpO₂";
  label_spo2_graph.color(TFT_BLACK);
  label_spo2_graph.font(prasanmit_20);
  label_spo2_graph.align(ALIGN_CENTER);

  // Vital Boxes (Right)
  graph_resp_bg.parent(rect_Page2);
  graph_resp_bg.size(142, 60);
  graph_resp_bg.color(TFT_RED);
  graph_resp_bg.corner_radius(20);
  graph_resp_bg.align(ALIGN_TOP_RIGHT, -5, 5);
  label_pr_title.parent(graph_resp_bg);
  label_pr_title = "PR";
  label_pr_title.color(TFT_BLACK);
  label_pr_title.font(prasanmit_20);
  label_pr_title.align(ALIGN_TOP_LEFT, 10, 5);
  btn_pr_value.parent(graph_resp_bg);
  btn_pr_value.size(90, 25);
  btn_pr_value.color(TFT_WHITE);
  btn_pr_value.text_color(TFT_BLACK);
  btn_pr_value = String(pr) + " BPM";
  btn_pr_value.align(ALIGN_IN_RIGHT_MID, -10, 0);

  graph_temp_bg.parent(rect_Page2);
  graph_temp_bg.size(142, 45);
  graph_temp_bg.color(TFT_CYAN);
  graph_temp_bg.corner_radius(20);
  graph_temp_bg.align(graph_resp_bg, ALIGN_OUT_BOTTOM_MID, 0, 5);
  label_temp_title.parent(graph_temp_bg);
  label_temp_title = "Temp.";
  label_temp_title.color(TFT_BLACK);
  label_temp_title.font(prasanmit_20);
  label_temp_title.align(ALIGN_TOP_LEFT, 10, 3);
  btn_temp_value.parent(graph_temp_bg);
  btn_temp_value.size(80, 30);
  btn_temp_value.color(TFT_WHITE);
  btn_temp_value.text_color(TFT_BLACK);
  btn_temp_value = String(temp, 1) + "°C";
  btn_temp_value.align(ALIGN_IN_RIGHT_MID, -10, 0);

  graph_spo2_bg.parent(rect_Page2);
  graph_spo2_bg.size(142, 60);
  graph_spo2_bg.color(TFT_YELLOW);
  graph_spo2_bg.corner_radius(20);
  graph_spo2_bg.align(graph_temp_bg, ALIGN_OUT_BOTTOM_MID, 0, 5);
  label_spo2_title.parent(graph_spo2_bg);
  label_spo2_title = "SpO₂";
  label_spo2_title.color(TFT_BLACK);
  label_spo2_title.font(prasanmit_20);
  label_spo2_title.align(ALIGN_TOP_LEFT, 10, 3);
  btn_spo2_value.parent(graph_spo2_bg);
  btn_spo2_value.size(80, 25);
  btn_spo2_value.color(TFT_WHITE);
  btn_spo2_value.text_color(TFT_BLACK);
  btn_spo2_value = String(spo2) + "%";
  btn_spo2_value.align(ALIGN_IN_RIGHT_MID, -10, 0);

  graph_bp_bg.parent(rect_Page2);
  graph_bp_bg.size(142, 115);
  graph_bp_bg.color(TFT_GREEN);
  graph_bp_bg.corner_radius(20);
  graph_bp_bg.align(graph_spo2_bg, ALIGN_OUT_BOTTOM_MID, 0, 5);
  label_bp_title.parent(graph_bp_bg);
  label_bp_title = "BP";
  label_bp_title.color(TFT_BLACK);
  label_bp_title.font(prasanmit_20);
  label_bp_title.align(ALIGN_TOP_LEFT, 10, 5);
  btn_sys_value.parent(graph_bp_bg);
  btn_sys_value.size(135, 30);
  btn_sys_value.color(TFT_WHITE);
  btn_sys_value.text_color(TFT_BLACK);
  btn_sys_value.font(prasanmit_25);
  btn_sys_value = "SYS: " + String(sys) + " mmHg";
  btn_sys_value.align(ALIGN_CENTER, 0, -15);
  btn_dia_value.parent(graph_bp_bg);
  btn_dia_value.size(135, 30);
  btn_dia_value.color(TFT_WHITE);
  btn_dia_value.text_color(TFT_BLACK);
  btn_dia_value.font(prasanmit_25);
  btn_dia_value = "DIA: " + String(dia) + " mmHg";
  btn_dia_value.align(ALIGN_CENTER, 0, 25);

  label_time.parent(rect_Page2);
  label_time = "00:00";
  label_time.color(TFT_BLACK);
  label_time.font(prasanmit_25);
  label_time.align(graph_bp_bg, ALIGN_OUT_BOTTOM_RIGHT, -6, -7);
  label_date.parent(rect_Page2);
  label_date = "2025-01-01";
  label_date.color(TFT_BLACK);
  label_date.font(prasanmit_25);
  label_date.align(graph_bp_bg, ALIGN_OUT_BOTTOM_LEFT, 0, -7);

  btn_back_p2.parent(rect_Page2);
  btn_back_p2 = SYMBOL_ARROW_LEFT;
  btn_back_p2.size(60, 60);
  btn_back_p2.round_design();
  btn_back_p2.color(TFT_GRAY);
  btn_back_p2.align(ALIGN_TOP_LEFT, 10, 10);
  btn_back_p2.onClicked([](GWidget* widget) {
    rect_Page2.hidden(true);
    rect_Home.hidden(false);
  });
}

// ================= Loop =================
void loop() {
  BlynkGO.update();
}
