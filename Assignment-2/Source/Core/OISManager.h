#ifndef OISManager_H
#define OISManager_H
 
#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <OgreRenderWindow.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <SdkCameraMan.h>

class OISManager : public OIS::KeyListener, public OIS::MouseListener {
public:
    virtual ~OISManager( void );
 
    void initialise( Ogre::RenderWindow *renderWindow );
    void setupCameraMan(OgreBites::SdkCameraMan * camMan);
    void capture( void );
 
    void addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName );
    void addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName );
 
    void removeKeyListener( const std::string& instanceName );
    void removeMouseListener( const std::string& instanceName );

    void removeKeyListener( OIS::KeyListener *keyListener );
    void removeMouseListener( OIS::MouseListener *mouseListener );
 
    void removeAllListeners( void );
    void removeAllKeyListeners( void );
    void removeAllMouseListeners( void );
 
    void setWindowExtents( int width, int height );
 
    OIS::Mouse*    getMouse( void );
    OIS::Keyboard* getKeyboard( void );
    
    OgreBites::SdkCameraMan * cameraMan;
 
    static OISManager* getSingletonPtr( void );
    


    int getMouseXAxis();
    int getMouseYAxis();
    OIS::KeyCode getKeyPressed();
    OIS::KeyCode lastKeyPressed();

private:
    OISManager( void );
    OISManager( const OISManager& ) { }
    OISManager & operator = ( const OISManager& );
 
    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );
 
    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    
    CEGUI::MouseButton convertButton(OIS::MouseButtonID);

    bool killWindow(bool running);
 
    OIS::Mouse        *mMouse;
    OIS::Keyboard     *mKeyboard;
    OIS::InputManager *mInputSystem;
 
    std::map<std::string, OIS::KeyListener*> mKeyListeners;
    std::map<std::string, OIS::MouseListener*> mMouseListeners;
 
    std::map<std::string, OIS::KeyListener*>::iterator itKeyListener;
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListener;
 
    std::map<std::string, OIS::KeyListener*>::iterator itKeyListenerEnd;
    std::map<std::string, OIS::MouseListener*>::iterator itMouseListenerEnd;
 
    static OISManager *mOISManager;

    // Our private members here
    int mouseXAxis;
    int mouseYAxis;
    OIS::KeyCode mKeyPressed;
};
#endif