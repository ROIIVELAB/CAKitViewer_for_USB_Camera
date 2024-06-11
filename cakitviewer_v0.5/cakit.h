#include <gtkmm.h>
#include <gtkmm/application.h>
#include <iostream>
#include <string.h>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
using namespace Glib;
using namespace Gtk;

string gstreamer_pipeline (string camera_index, int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "v4l2src " + camera_index + " ! video/x-raw, width=(int)" + to_string(capture_width) + ", height=(int)" +
           to_string(capture_height) + ",format=UYVY, framerate=(fraction)" + to_string(framerate) +
           "/1 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

class WindowMain : public Window {
public:
  WindowMain() {

    /////////////////////////////////////////////////////////////////////////////////////////////
    // UI 생성

    add(scrolledWindow);
    scrolledWindow.add(fixed);

    label_index.set_text("Camera Index");
    fixed.add(label_index);
    fixed.move(label_index, 20, 45);

    index.set_text("0");
    index.set_width_chars(1);
    fixed.add(index);
    fixed.move(index, 120, 40);

    cakit_start.set_label(" CAkit 시작 ");
    fixed.add(cakit_start);
    fixed.move(cakit_start, 5, 90);

    cakit_end.set_label(" CAkit 종료 ");
    fixed.add(cakit_end);
    fixed.move(cakit_end, 100, 90);
 
    save_image.set_label("                   한장 캡쳐                  ");
    fixed.add(save_image);
    fixed.move(save_image, 5, 130);

    label_resize.set_text("==== Input/Output Size ====");
    fixed.add(label_resize);
    fixed.move(label_resize, 200, 50);
    
    resize_check.set_label("Resize On/Off");
    resize_check.set_active(false);
    fixed.add(resize_check);
    fixed.move(resize_check, 210, 75);

    label_input_size.set_text("Input     Size : ");
    fixed.add(label_input_size);
    fixed.move(label_input_size, 225, 105);

    label_output_size.set_text("Output Size : ");
    fixed.add(label_output_size);
    fixed.move(label_output_size, 225, 125);

    label_log.set_text("[Log]");
    fixed.add(label_log);
    fixed.move(label_log, 420, 20);

    Log.set_size_request(310,140);
    fixed.add(Log);
    fixed.move(Log,423,45);
    
    log_clear.set_label("지우기");
    fixed.add(log_clear);
    fixed.move(log_clear, 680, 190);

    label_febd.set_text("[Front Embedded Data]");
    fixed.add(label_febd);
    fixed.move(label_febd, 5, 220);

    febd_check.set_label("FEBD On/Off");
    febd_check.set_active(false);
    fixed.add(febd_check);
    fixed.move(febd_check, 20, 240);

    label_folder_dir.set_text("[Capture 폴더 위치]");
    fixed.add(label_folder_dir);
    fixed.move(label_folder_dir, 10, 715);

    folder_dir.set_text("");
    folder_dir.set_width_chars(89);
    fixed.add(folder_dir);
    fixed.move(folder_dir, 10, 740);

    select_folder.set_label("     폴더 선택    ");
    fixed.add(select_folder);
    fixed.move(select_folder, 530, 770);

    open_folder.set_label("저장 폴더 열기");
    fixed.add(open_folder);
    fixed.move(open_folder, 635, 770);

    label_ini_dir.set_text("[ini 파일 위치]");
    fixed.add(label_ini_dir);
    fixed.move(label_ini_dir, 10, 795);

    ini_dir.set_text("");
    ini_dir.set_width_chars(89);
    fixed.add(ini_dir);
    fixed.move(ini_dir, 10, 820);

    select_ini.set_label("  ini파일 선택  ");
    fixed.add(select_ini);
    fixed.move(select_ini, 530, 850);

    open_ini.set_label("   ini파일 열기  "); 
    fixed.add(open_ini);
    fixed.move(open_ini, 635, 850);

    set_title("CAKitViewer for USB Interface");
    resize(750, 900);
    show_all();

/////////////////////////////////////////////////////////////////////////////////////////////
// 버튼 or 체크박스 클릭 시
    
    cakit_start.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_cakit_start) );

    cakit_end.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_cakit_end) );

    resize_check.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_resize_check) );     

    save_image.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_save_image) );

    log_clear.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_log_clear) );

    select_folder.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_select_folder) );           

    open_folder.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_open_folder) );

    select_ini.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_select_ini) );

    open_ini.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_open_ini) );                       
  }

public :
Mat img, resize_img;
VideoCapture cap;
string camera_index;
int width_input=0;
int height_input=0;
int width_output=0;
int height_output=0;
bool bReturn = 0;
bool bChecked = 0;
void camera_in();
void resize_size();
void on_button_clicked_cakit_start();
void on_button_clicked_cakit_end();
void on_button_clicked_resize_check();
void on_button_clicked_log_clear();
void on_button_clicked_save_image();
void on_button_clicked_select_folder();
void on_button_clicked_open_folder();
void on_button_clicked_select_ini();
void on_button_clicked_open_ini();

private:
  Fixed fixed;
  ScrolledWindow scrolledWindow;
  Button cakit_start;
  Button cakit_end;
  Button save_image;
  Button log_clear;
  Button select_folder;
  Button open_folder;
  Button select_ini;
  Button open_ini;

  CheckButton resize_check;
  CheckButton febd_check;

  Entry index;
  Entry folder_dir;
  Entry ini_dir;

  TextView Log;

  Label label_index;
  Label label_resize;
  Label label_input_size;
  Label label_output_size;
  Label label_input_size_2;
  Label label_output_size_2;
  Label label_log;
  Label label_febd;
  Label label_folder_dir;
  Label label_ini_dir;

  int cam_startClicked = 0;
  int cam_endClicked = 0;
  int save_imageClicked = 0;
  int log_clearClicked = 0;
  int select_folderClicked = 0;
  int open_folderClicked = 0;
  int select_iniClicked = 0;
  int open_iniClicked = 0;
};