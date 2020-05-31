#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "experimental/filesystem"

using namespace ci;
using namespace ci::app;
using namespace std;

class cataLogApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void keyDown( KeyEvent event) override;
	void update() override;
	void draw() override;
	void prepare_settings(App::Settings* settings);
	gl::Texture2dRef myImage;
	int img_width;
	int img_height;
	int click_x = -1;
	int click_y = -1;
	bool clicked = false;

	string current_filename;
	int current_fileindex;
	vector<string> filenames;
	bool changefile = false;
};

std::vector<std::string> get_filenames(std::experimental::filesystem::path path) {
  namespace stdfs = std::experimental::filesystem;
  std::vector<std::string> filenames;
  const stdfs::directory_iterator end{} ;
  for( stdfs::directory_iterator iter{path} ; iter != end ; ++iter ) {
    if( stdfs::is_regular_file(*iter) )
      filenames.push_back( iter->path().filename().string());
  }
  return filenames;
}

void prepare_settings(App::Settings* settings) {
  const int width = 750;
  const int height = 500;
  settings->setWindowSize(width, height);
  settings->setResizable(false);
  settings->setTitle("cataLog");
  settings->setFrameRate(60.0f);
}

void cataLogApp::setup() {
  filenames = get_filenames("..\\..\\..\\assets\\");
  current_fileindex = 0;
  myImage = gl::Texture2d::create(loadImage(loadAsset(filenames[current_fileindex])));
  img_width = myImage->getWidth();
  img_height = myImage->getHeight();
}

void cataLogApp::mouseDown( MouseEvent event ) {
  click_x = event.getX();
  click_y = event.getY();
  clicked = true;
}

void cataLogApp::keyDown(KeyEvent event) {
  if (event.getChar() == '.') {
    if (current_fileindex < filenames.size() - 1) {
      current_fileindex += 1;
      changefile = true;
    }
  }
  if (event.getChar() == ',') {
    if (current_fileindex > 0) {
      current_fileindex -= 1;
      changefile = true;
    }
  }
}

void cataLogApp::update()
{
  if (changefile) {
    current_filename = filenames[current_fileindex];
    myImage = gl::Texture2d::create(
        loadImage(loadAsset(filenames[current_fileindex])));
    changefile = false;
  }
}

void cataLogApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );
	gl::draw(myImage, getWindowBounds());
	if (clicked) {
          gl::drawSolidCircle({click_x, click_y}, 2);
        }
        gl::drawString(current_filename, {getWindowWidth()-50,0});
	gl::drawString("X: " + to_string(click_x), {getWindowWidth()-50,15});
        gl::drawString("Y: " + to_string(click_y), {getWindowWidth()-50,30});
        gl::drawString("%X!: " + to_string((double) click_x/632), {getWindowWidth()-75,45});
        gl::drawString("%Y: " + to_string((double) click_y/getWindowHeight()), {getWindowWidth()-75,60});
        gl::drawLine({632,0},{632,500});
}

CINDER_APP(cataLogApp, RendererGl, prepare_settings)
