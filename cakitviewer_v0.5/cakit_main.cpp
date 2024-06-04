#include "cakit.h"


void WindowMain::camera_in(){

int capture_width = 1920 ;
int capture_height = 1551 ;
int display_width = 1920 ;
int display_height = 1551 ;
int framerate = 30 ;
int flip_method = 0 ;

string pipeline = gstreamer_pipeline(camera_index,capture_width, capture_height,
display_width, display_height, framerate, flip_method);

  cap.open(pipeline,CAP_GSTREAMER);

      if(!cap.isOpened()) {
        cout<<"Failed to open camera."<<endl;
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

      while(true)
      {
        if (!cap.read(img)) {
          cout<<"Capture read error"<<endl;
          break;
        }

        if(bChecked==TRUE){

          cv::resize(img,resize_img,Size(width_output,height_output),0,0,INTER_LINEAR);
          imshow("USB Camera - GStreamer",resize_img);
        }
        else{
          imshow("USB Camera - GStreamer",img);
        }
    
        if (waitKey(10) == 27) break ;

        if(bReturn == TRUE){
          cap.release();
          break;
        }
      }
      
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