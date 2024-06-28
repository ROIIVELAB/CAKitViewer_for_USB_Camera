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

void WindowMain::on_button_clicked_febd_log_clear(){
  Log_buffer->set_text("");
  FEBD_Log.set_buffer(Log_buffer);
  febd_buffer = Log_buffer->get_text();
}

void WindowMain::Listout(int log, string message){

  switch(log)
  {
    case 1:
      Log_buffer->set_text(buffer+message);
      Log.set_buffer(Log_buffer);
      buffer = Log_buffer->get_text();
      break;

    case 2:
      Log_buffer->set_text(febd_buffer+message);
      FEBD_Log.set_buffer(Log_buffer);
      febd_buffer = Log_buffer->get_text();
      break;

  }
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
    Listout(1," USB 케이블 or USB전원 재연결 필요[0]\n");
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

//  bTrue = cap.set(cv::CAP_PROP_CONVERT_RGB, 0.0f);
//	if(!bTrue) Listout(1,"Warning:CAP_PROP_CONVERT_RGB");

  while(true){

    if (cap.grab()){
      if (!cap.retrieve(rawframe)) {

        cap.release();
				sleep(10);
        cap.open(pipeline, CAP_GSTREAMER);

        if (cap.isOpened()) {
					sleep(10);
          Listout(1," 영상이 정상 수신되지 않았습니다.[1]\n");

					continue;
				}
				else {
					cap.release();
					sleep(10);
          Listout(1," USB 케이블 or USB전원 재연결 필요[1]\n");

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
    	    Listout(1," 영상이 정상 수신되지 않았습니다.[2]\n");
      
			continue;
		}
		else {
			cap.release();
			sleep(10);
        	Listout(1," USB 케이블 or USB전원 재연결 필요[2]\n");

			break;
		}
	}

//    Listout(2,"febd");

    uchar *pImgData = (uchar*)rawframe.data;
	unsigned int tmp = 0;

	if(febd_check.get_active()==TRUE)
	{
		/*
        FEBD_str = to_string(pImgData[36])+to_string(pImgData[37])+to_string(pImgData[38])+to_string(pImgData[39])+ " , " + // Frame Count
        to_string(pImgData[120])+to_string(pImgData[121])+to_string(pImgData[122])+to_string(pImgData[123])+ " , " + // Analog Gain
        to_string(pImgData[124])+to_string(pImgData[125])+to_string(pImgData[126])+to_string(pImgData[127])+ " , " +
        to_string(pImgData[140])+to_string(pImgData[141])+to_string(pImgData[142])+ " , " + // Digital Gain
        to_string(pImgData[30])+to_string(pImgData[31])+to_string(pImgData[32])+to_string(pImgData[33])+to_string(pImgData[34])+to_string(pImgData[35])+ " , " +	// Exposure
        to_string(pImgData[20])+to_string(pImgData[21])+to_string(pImgData[22])+to_string(pImgData[23])+	// Host Select
        to_string(pImgData[24])+to_string(pImgData[25])+to_string(pImgData[26])+to_string(pImgData[27]);
        
        Listout(2,FEBD_str);
		Listout(2,"\n");
		*/

	 	FEBD_str = to_string(pImgData[36])+to_string(pImgData[37])+to_string(pImgData[38])+to_string(pImgData[39])+ "\n"; // Frame Count
        Listout(2,FEBD_str);
    }
	  
/*
				// Frame Count
				tmp = pImgData[39];
				tmp = tmp << 8 | pImgData[38];
				tmp = tmp << 8 | pImgData[37];
				tmp = tmp << 8 | pImgData[36];
				frame_cnt_num = tmp;

				FEBD_str.Format(_T(" %03d"), frame_cnt_num);
			//	SetDlgItemText(IDC_FRAME_CNT, FEBD_str);

				// Analog Gain
				tmp = pImgData[121];
				tmp = tmp << 8 | pImgData[120];
				analog_gain_num = tmp;

				FEBD_str.Format(_T(" %02d"), analog_gain_num / 10);
			//	SetDlgItemText(IDC_AGAIN_SP1_H, FEBD_str);


				tmp = pImgData[123];
				tmp = tmp << 8 | pImgData[122];
				analog_gain_num = tmp;

				FEBD_str.Format(_T(" %02d"), analog_gain_num / 10);
			//	SetDlgItemText(IDC_AGAIN_SP1_L, FEBD_str);


				tmp = pImgData[125];
				tmp = tmp << 8 | pImgData[124];
				analog_gain_num = tmp;

				FEBD_str.Format(_T(" %02d"), analog_gain_num / 10);
			//	SetDlgItemText(IDC_AGAIN_SP2_H, FEBD_str);


				tmp = pImgData[127];
				tmp = tmp << 8 | pImgData[126];
				analog_gain_num = tmp;

				FEBD_str.Format(_T(" %02d"), analog_gain_num / 10);
			//	SetDlgItemText(IDC_AGAIN_SP2_L, FEBD_str);


				// Digital Gain
				tmp = pImgData[142];
				tmp = tmp << 8 | pImgData[141];
				digital_gain_num = (double)tmp;

				tmp = pImgData[140];
				digital_gain_num2 = tmp / 10;
				digital_gain_num3 = tmp % 10;

				square1 = pow(16, -1);
				square2 = pow(16, -2);

				digital_gain_calc = digital_gain_num + digital_gain_num2 * square1 + digital_gain_num3 * square2;
				FEBD_str.Format(_T(" %0.2f"), 20 * log10(digital_gain_calc));
			//	SetDlgItemText(IDC_digital_gain, FEBD_str);


				// Exposure Control
				tmp = pImgData[32];
				tmp = tmp << 8 | pImgData[31];
				tmp = tmp << 8 | pImgData[30];
				exposure_num = (double)tmp;

				FEBD_str.Format(_T(" %0.2f"), exposure_num / 30 / 1750 * 1000);			// 단위 line -> ms로 변경
			//	SetDlgItemText(IDC_SHT_SP1, FEBD_str);								// 1 line[s] = 1/frame rate(30)/VMAX(1750)


				tmp = pImgData[35];
				tmp = tmp << 8 | pImgData[34];
				tmp = tmp << 8 | pImgData[33];
				exposure_num = (double)tmp;

				FEBD_str.Format(_T(" %0.2f"), exposure_num / 30 / 1750 * 1000);
			//	SetDlgItemText(IDC_SHT_SP2, FEBD_str);


				// H/V Flip
				sprintf_s(tmp_buf3, " 0x%02X", pImgData[168]);
			//	SetDlgItemText(IDC_HFlip, tmp_buf3);

				sprintf_s(tmp_buf3, " 0x%02X", pImgData[169]);
			//	SetDlgItemText(IDC_VFlip, tmp_buf3);


				// Output Size
				tmp = pImgData[189];
				tmp = tmp << 8 | pImgData[188];
				HOUT_num = tmp;

				FEBD_str.Format(_T(" 0x%04X (%03d)"), HOUT_num, HOUT_num);
			//	SetDlgItemText(IDC_OUTPUT_H, FEBD_str);


				tmp = pImgData[191];
				tmp = tmp << 8 | pImgData[190];
				VOUT_num = tmp;

				FEBD_str.Format(_T(" 0x%04X (%03d)"), VOUT_num, VOUT_num);
			//	SetDlgItemText(IDC_OUTPUT_V, FEBD_str);


				// Host Selected Registers
				tmp = pImgData[21];
				tmp = tmp << 8 | pImgData[20];
				H_AGAIN_num = tmp;

				H_str.Format(_T(" %02d"), H_AGAIN_num / 10);
			//	SetDlgItemText(IDC_H_AGAIN, H_str);


				tmp = pImgData[23];
				tmp = tmp << 8 | pImgData[22];
				H_DGAIN_num = (double)tmp;

				H_str.Format(_T(" %0.1f"), H_DGAIN_num / 10);
			//	SetDlgItemText(IDC_H_DGAIN, H_str);


				sprintf_s(tmp_buf3, "%02x", pImgData[24]);
				H_AELEVEL_num = strtol(tmp_buf3, NULL, 16);
				H_str.Format(_T(" %0.2f"), H_AELEVEL_num*0.7525);
			//	SetDlgItemText(IDC_H_AELEVEL, H_str);


				H_ILMLEVEL_num = pImgData[25];
				H_str.Format(_T(" %0.2f"), H_ILMLEVEL_num*0.7525);
			//	SetDlgItemText(IDC_H_ILMLEVEL, H_str);


				tmp = pImgData[27];
				tmp = tmp << 8 | pImgData[26];
				H_EXPOSURE_num = (double)tmp;

				H_str.Format(_T(" %0.1f"), H_EXPOSURE_num / 10);
			//	SetDlgItemText(IDC_H_EXPOSURE, H_str);


				H_DARK_LEVEL_num = pImgData[28];
				H_str.Format(_T(" %0.1f"), H_DARK_LEVEL_num*0.3);
			//	SetDlgItemText(IDC_H_DARKLEVEL, H_str);


				E30 = pImgData[29];
				H_str.Format(_T(" %d"), E30);
			//	SetDlgItemText(IDC_H_E30, H_str);
    }
*/
/*		frame.create(rawframe.size(), CV_8UC3);
		cvtColor(rawframe, frame, COLOR_YUV2BGR_UYVY);
*/   
    if(bChecked==TRUE){

      cv::resize(rawframe,resize_img,Size(width_output,height_output),0,0,INTER_LINEAR);
      imshow("USB Camera - GStreamer",resize_img);
    }
    else{
      imshow("USB Camera - GStreamer",rawframe);
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