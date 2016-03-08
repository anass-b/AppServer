#include <iostream>
#include <nativecode/nativecode.h>
#include <time.h>

using namespace app;

#define kRedView 10
#define kGreenView 20
#define kBlueView 30
#define kYellowView 40
#define kGrayView 50
#define kBlackView 60

class ColoredView : public View
{
public:
    ColoredView(Rect frame, Color bg) : View(frame)
    {
        _background = bg;
    }
protected:
    virtual void draw()
    {
        getSurface()->fill(makeColor(_background.r, _background.g, _background.b));
    }
private:
    Color _background;
};

class ImageView : public View
{
public:
    ImageView(Rect frame, const char *filename) : View(frame)
    {
        _imageSurface = new Surface(filename);
    }
protected:
    virtual void draw()
    {
        getSurface()->blit(_imageSurface, makeRect(0.0, 0.0, _imageSurface->getSize().width, _imageSurface->getSize().height), makePoint(0.0, 0.0));
    }
private:
    Surface *_imageSurface;
};

class TestApplicationDelegate : public ApplicationDelegate, MouseEventDelegate, KeyEventDelegate
{
public:
    virtual void applicationStarted()
    {
        _secondWindow = new Window(makeRect(200.0, 200.0, 640.0, 480.0), kWindowVisibilityVisible);
//        _secondWindow->setWindowManagementEnabled(false);
        //_secondWindow->addKeyEventDelegate(this);
        //_secondWindow->addMouseEventDelegate(this);
        _imageView2 = createLandscapeImageView(_secondWindow, "img/space.png");
        
        /*_thirdWindow =  new Window(makeRect(400.0, 400.0, 400, 300), kWindowVisibilityVisible);
        _thirdWindow->addMouseEventDelegate(this);
        _thirdWindow->addKeyEventDelegate(this);
        createRedView(_thirdWindow);
        createGreenView(_thirdWindow);*/
        
        _mainWindow = new Window(makeRect(50.0, 50.0, 480.0, 640.0), kWindowVisibilityVisible);
        //_mainWindow->addMouseEventDelegate(this);
        //_mainWindow->addKeyEventDelegate(this);
        _imageView1 = createPortraitImageView(_mainWindow, "img/image.png");
    }
    
    static void* highWorkloadTest(void *ptr)
    {
        TestApplicationDelegate *delegate = (TestApplicationDelegate*)ptr;
        
        double x = 0;
        double y = 0;
        while (delegate->_highWorkloadDrawingEnabled) {
            x+=50;
            y+=50;
            if (x > 640) x = 0;
            if (y > 480) y = 0;
            delegate->_imageView1->move(makePoint(x, y));
        }
        
        return nullptr;
    }
    
    void createRedView(Window *w)
    {
        ColoredView *v = new ColoredView(makeRect(80.0, 80.0, 100.0, 100.0), makeColor(255, 0, 0));
        v->setTag(kRedView);
        v->addMouseEventDelegate(this);
        
        ColoredView *sv = new ColoredView(makeRect(-10.0, -10.0, 20.0, 20.0), makeColor(0, 255, 255));
        sv->setTag(kYellowView);
        sv->addMouseEventDelegate(this);
        v->addSubView(sv);
        
        w->addView(v);
    }
    
    void createGreenView(Window *w)
    {
        View *mGreenView = new ColoredView(makeRect(50.0, 50.0, 100.0, 100.0), makeColor(0, 255, 0));
        mGreenView->setTag(kGreenView);
        mGreenView->addMouseEventDelegate(this);
        
        ColoredView *sv = new ColoredView(makeRect(50.0, 50.0, 100.0, 100.0), makeColor(128, 128, 128));
        sv->setTag(kGrayView);
        sv->addMouseEventDelegate(this);
        mGreenView->addSubView(sv);
        
        ColoredView *sv2 = new ColoredView(makeRect(-80.0, -80.0, 100.0, 100.0), makeColor(0, 0, 0));
        sv2->setTag(kBlackView);
        sv2->addMouseEventDelegate(this);
        sv->addSubView(sv2);
        
        w->addView(mGreenView);
    }
    
    ImageView* createImageView(Window *w, const char *filename)
    {
        ImageView *imageView = new ImageView(makeRect(70.0, 70.0, 350.0, 382.0), filename);
        imageView->setTag(kGreenView);
        imageView->addMouseEventDelegate(this);
        w->addView(imageView);
        
        return imageView;
    }
    
    ImageView* createLandscapeImageView(Window *w, const char *filename)
    {
        ImageView *imageView = new ImageView(makeRect(0.0, 0.0, 640.0, 480.0), filename);
        w->addView(imageView);
        
        return imageView;
    }
    
    ImageView* createPortraitImageView(Window *w, const char *filename)
    {
        ImageView *imageView = new ImageView(makeRect(0.0, 0.0, 480.0, 640.0), filename);
        w->addView(imageView);
        
        return imageView;
    }
    
    virtual void mouseEventOccured(MouseEvent *e)
    {
        if (e == nullptr) return;
        if (e->type == kMouseEventTypePress) {
            if (e->viewTag == kRedView) {
                std::cout << "Red" << std::endl;
            }
            else if (e->viewTag == kGreenView) {
                std::cout << "Green" << std::endl;
            }
            else if (e->viewTag == kYellowView) {
                std::cout << "Yellow" << std::endl;
            }
            else if (e->viewTag == kGrayView) {
                std::cout << "Gray" << std::endl;
            }
            else if (e->viewTag == kBlackView) {
                std::cout << "Black" << std::endl;
            }
            
//            if (e->winId == _secondWindow->getId()) {
//                Size newSize = _secondWindow->getSize();
//                newSize.width += 10.0;
//                newSize.height += 10.0f;
//                
//                if (newSize.width >= 1000.0 || newSize.height >= 1000.0) {
//                    newSize.width = 320.0;
//                    newSize.height = 240.0f;
//                }
//                _secondWindow->resizeWindow(newSize);
//            }
            
        }
        else if (e->type == kMouseEventTypeMove) {
            if (e->viewTag == -1) {
                if (e->winId == _mainWindow->getId()) {
                    if (!_highWorkloadDrawingEnabled) {
                        _mainWindow->getContentView()->getSurface()->fill(makeColor(255, 255, 255, 0));
                        Size size1 = _imageView1->getFrame().size;
                        _imageView1->move(makePoint(e->x - size1.width / 2, e->y - size1.height / 2));
                    }
                }
                else if (e->winId == _secondWindow->getId()) {
                    _secondWindow->getContentView()->getSurface()->fill(makeColor(255, 255, 255, 0));
                    Size size2 = _imageView2->getFrame().size;
                    _imageView2->move(makePoint(e->x - size2.width / 2, e->y - size2.height / 2));
                }
            }
            
        }
    }
    
    virtual void keyEventOccured(KeyEvent *e)
    {
//        double x = randomNumber(0, 900);
//        double y = randomNumber(0, 500);
//        new Window(makeRect(x, y, 320, 240), kWindowVisibilityVisible);
        
        if (e->charCode == 'h') {
            if (_highWorkloadDrawingEnabled) {
                _highWorkloadDrawingEnabled = false;
            }
            else {
                _highWorkloadDrawingEnabled = true;
                
                pthread_t thread;
                if (pthread_create(&thread, NULL, TestApplicationDelegate::highWorkloadTest, this)) {
                    throw std::runtime_error(strerror(errno));
                }
            }
        }
    }
    
    int randomNumber(int min_num, int max_num)
    {
        int result=0,low_num=0,hi_num=0;
        if(min_num<max_num)
        {
            low_num=min_num;
            hi_num=max_num+1; // this is done to include max_num in output.
        }
        else {
            low_num=max_num+1;// this is done to include max_num in output.
            hi_num=min_num;
        }
        srand((unsigned int)time(NULL));
        result = (rand()%(hi_num-low_num))+low_num;
        return result;
    }
private:
    ImageView* _imageView1 = nullptr;
    ImageView* _imageView2 = nullptr;
    Window* _mainWindow;
    Window* _secondWindow;
    Window* _thirdWindow;
    bool _highWorkloadDrawingEnabled = false;
};

int main(int argc, const char * argv[])
{
    return Application::getSingleton()->run(new TestApplicationDelegate());
}
