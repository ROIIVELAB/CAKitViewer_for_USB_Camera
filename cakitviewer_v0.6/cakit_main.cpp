#include "cakit.h"

void WindowMain::on_button_clicked_cakit_start(){
  bReturn = FALSE;
      camera_index = index.get_text();

      if(camera_index=="0"){
          camera_index = "device=/dev/video0";
      }
      else if(camera_index =="1"){
        camera_index = "device=/dev/video1";
      }

      camera_in();
}

void WindowMain::on_button_clicked_cakit_end(){
  bReturn = TRUE;
}

void WindowMain::on_button_clicked_resize_check(){
  resize_size();
}

void WindowMain::on_button_clicked_log_clear(){
  Log_buffer->set_text("");
  Log.set_buffer(Log_buffer);
  buffer = Log_buffer->get_text();
}

void WindowMain::Listout(string message){

  Log_buffer->set_text(buffer+message);
  Log.set_buffer(Log_buffer);
  buffer = Log_buffer->get_text();

}

string gstreamer_pipeline (string camera_index, int capture_width, int capture_height, int framerate) {
    return "v4l2src " + camera_index + " ! video/x-raw, width=(int)" + to_string(capture_width) + ", height=(int)" +
           to_string(capture_height) + ",format=UYVY, framerate=(fraction)" + to_string(framerate) +
           "/1 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

void WindowMain::camera_in(){

  int capture_width = 1920 ;
  int capture_height = 1551 ;
  int framerate = 30 ;

  string pipeline = gstreamer_pipeline(camera_index,capture_width, capture_height,framerate);

  cap.open(pipeline,CAP_GSTREAMER);

  if(!cap.isOpened()) {
    cap.release();
    sleep(10);
    Listout(" USB 케이블 or USB전원 재연결 필요[0]\n");
    return;
  }	

  width_input = cap.get(CAP_PROP_FRAME_WIDTH);
  height_input = cap.get(CAP_PROP_FRAME_HEIGHT);
      
  label_input_size.set_text("Input     Size : 1920 * 1551");
  fixed.add(label_input_size);
    
  if(bChecked==TRUE){
      
    label_output_size.set_text("Output Size : 1280 * 1034");
    fixed.add(label_output_size);
  }
  else{
    label_output_size.set_text("Output Size : 1920 * 1551");
    fixed.add(label_output_size);
  }
      
  namedWindow("USB Camera - GStreamer", WINDOW_AUTOSIZE);
      
  Log_buffer = Gtk::TextBuffer::create();

  while(true){

    if (cap.grab()){
      if (!cap.retrieve(img)) {

        cap.release();
				sleep(10);
        cap.open(pipeline, CAP_GSTREAMER);

        if (cap.isOpened()) {
					sleep(10);
          Listout(" 영상이 정상 수신되지 않았습니다.[1]\n");

					continue;
				}
				else {
					cap.release();
					sleep(10);
          Listout(" USB 케이블 or USB전원 재연결 필요[1]\n");

					break;
        }
      }
    }
		else {

			cap.release();
			sleep(10);

			cap.open(pipeline, CAP_GSTREAMER);

			if (cap.isOpened()) {
				sleep(10);
        Listout(" 영상이 정상 수신되지 않았습니다.[2]\n");
      
				continue;
			}
			else {
				cap.release();
				sleep(10);
        Listout(" USB 케이블 or USB전원 재연결 필요[2]\n");

				break;
			}
		}

    if(bChecked==TRUE){

      cv::resize(img,resize_img,Size(width_output,height_output),0,0,INTER_LINEAR);
      imshow("USB Camera - GStreamer",resize_img);
    }
    else{
      imshow("USB Camera - GStreamer",img);
    }
    
    if (waitKey(10) == 27) break ;
    
    if(bReturn == TRUE) break;
  }

  cap.release();
  destroyAllWindows() ;
}

void WindowMain::resize_size(){

  if(resize_check.get_active()==TRUE){

    width_output = 1280;
    height_output = 1034;
    bChecked = TRUE;

    label_input_size.set_text("Input     Size : 1920 * 1551");
    fixed.add(label_input_size);
    label_output_size.set_text("Output Size : 1280 * 1034");
    fixed.add(label_output_size);
  }
  else {
    
    width_output = 1920;
    height_output = 1551;
    bChecked = FALSE;

    label_input_size.set_text("Input     Size : 1920 * 1551");
    fixed.add(label_input_size);
    label_output_size.set_text("Output Size : 1920 * 1551");
    fixed.add(label_output_size);
  }
}

int main(int argc, char* argv[]) {
  
  auto application = Application::create(argc, argv);
  WindowMain window;

  return application->run(window);
}