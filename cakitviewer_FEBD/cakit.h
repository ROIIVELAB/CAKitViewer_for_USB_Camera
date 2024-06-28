#include <gtkmm.h>
#include <gtkmm/application.h>
#include <iostream>
#include <string.h>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
using namespace Glib;
using namespace Gtk;

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

    FEBD_Log.set_size_request(718,250);
    fixed.add(FEBD_Log);
    fixed.move(FEBD_Log,15,450);
    
    log_clear.set_label("지우기");
    fixed.add(log_clear);
    fixed.move(log_clear, 680, 190);

    febd_log_clear.set_label("지우기");
    fixed.add(febd_log_clear);
    fixed.move(febd_log_clear,680, 705);

    label_febd.set_text("[Front Embedded Data]");
    fixed.add(label_febd);
    fixed.move(label_febd, 5, 220);

    febd_check.set_label("FEBD On/Off");
    febd_check.set_active(false);
    fixed.add(febd_check);
    fixed.move(febd_check, 20, 240);

    set_title("CAKitViewer for USB Interface");
    resize(750, 750);
    show_all();

/////////////////////////////////////////////////////////////////////////////////////////////
// 버튼 or 체크박스 클릭 시
    
    cakit_start.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_cakit_start) );
    cakit_end.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_cakit_end) );
    resize_check.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_resize_check) );                       
    log_clear.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_log_clear) ); 
    febd_log_clear.signal_clicked().connect( sigc::mem_fun(*this,
              &WindowMain::on_button_clicked_febd_log_clear) ); 
              
  }

public :
Mat rawframe,frame, resize_img;
VideoCapture cap;
string camera_index;
string FEBD_str;
bool bTrue=FALSE;
int width_input=0;
int height_input=0;
int width_output=0;
int height_output=0;
bool bReturn = 0;
bool bChecked = 0;
void camera_in();
void resize_size();
void Listout(int log, string message);
void on_button_clicked_cakit_start();
void on_button_clicked_cakit_end();
void on_button_clicked_resize_check();
void on_button_clicked_log_clear();
void on_button_clicked_febd_log_clear();


private:
  Fixed fixed;
  ScrolledWindow scrolledWindow;
  Button cakit_start;
  Button cakit_end;
  Button log_clear;
  Button febd_log_clear;
  
  CheckButton resize_check;
  CheckButton febd_check;

  Entry index;
  Entry folder_dir;
  Entry ini_dir;

  TextView Log;
  TextView FEBD_Log;
  Glib::RefPtr<TextBuffer> Log_buffer;
  string buffer;
  string febd_buffer;

  Label label_index;
  Label label_resize;
  Label label_input_size;
  Label label_output_size;
  Label label_input_size_2;
  Label label_output_size_2;
  Label label_log;
  Label label_febd;

  int cam_startClicked = 0;
  int cam_endClicked = 0;
  int log_clearClicked = 0;
  int febd_log_clearClicked = 0;
};