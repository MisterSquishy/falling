/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include "BlockHandler.h"
#include <time.h>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_Base {
    Play_State(const Play_State &);
    Play_State operator=(const Play_State &);
    
public:
	Play_State() : m_time_passed(0.0f), m_prev_clear_color(get_Video().get_clear_Color()), cs() {
        set_pausable(true);
        m_chrono.start();
        m_time = time(0);
    }
    
    ~Play_State()
    {
    }
    
    void on_key(const SDL_KeyboardEvent &event) {
        switch(event.keysym.sym) {
            case SDLK_UP:
            case SDLK_w:
                cs.jump = event.type == SDL_KEYDOWN;
                break;
                
			case SDLK_LEFT:
            case SDLK_a:
                cs.left = event.type == SDL_KEYDOWN;
                break;
                
            case SDLK_SPACE:
                cs.destroy = event.type == SDL_KEYDOWN;
                break;
                
			case SDLK_RIGHT:
            case SDLK_d:
                cs.right = event.type == SDL_KEYDOWN;
                break;
                
			case SDLK_RETURN:
				cs.retry = event.type == SDL_KEYDOWN;
				break;
                
			case SDLK_BACKSPACE:
				cs.main_menu = event.type == SDL_KEYDOWN;
				break;
                
            default:
                Gamestate_Base::on_key(event); // Let Gamestate_Base handle it
                break;
        }
    }
    
private:
    
    Color m_prev_clear_color;
    time_t m_time;
    
    void on_push() {
        //get_Window().mouse_grab(true);
        get_Window().mouse_hide(true);
        //get_Game().joy_mouse.enabled = false;
    }
    
    void on_pop() {
        //get_Window().mouse_grab(false);
        get_Window().mouse_hide(false);
        //get_Game().joy_mouse.enabled = true;
        
        get_Video().set_clear_Color(m_prev_clear_color);
    }
    
    Chronometer<Time> m_chrono;
    float m_time_passed;
    
	
    
    void perform_logic() {
        //time_t now = time(0);
        //float time_step = now - m_time;
        //time_step /= 100;
        //m_time = now;
        
		static float time_processed = 0.00f;
        
		const float time_passed = m_chrono.seconds();
        const float time_step = time_passed - m_time_passed;
        m_time_passed = time_passed;
        
		if(time_processed >= FRAME_RATE)
		{
			time_processed = 0.0f;
			bh.perform_logic(time_passed, FRAME_RATE, cs);
		}
		else
		{
			time_processed += time_step;
		}
        
		if(bh.getState() == BH_POST_DEATH)
		{
			m_chrono.reset();
			m_time_passed = 0.0f;
		}
    }
    
    void render() {
        get_Video().set_2d();
        //get_Video().set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true);
        
		bh.render();
    }
    
    BlockHandler bh;
    ControlState cs;
};



class Instructions_State : public Widget_Gamestate {
    Instructions_State(const Instructions_State &);
    Instructions_State operator=(const Instructions_State &);
    
public:
    Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
    {
        instridx = 0;
		get_Video().set_clear_Color(get_Colors()["green"]);
        m_chrono.start();
    }
    
private:
    int instridx;
    float lastTime;
    Chronometer<Time> m_chrono;
    
    void on_key(const SDL_KeyboardEvent &event) {
        
        if(event.type == SDL_KEYDOWN)
        {
            
            switch(event.keysym.sym) {
                case SDLK_LEFT:
                    if(m_chrono.seconds()-lastTime >= .5f)
                    {
                        instridx = max(instridx-1, 0);
                        lastTime = m_chrono.seconds();
                    }
                    
                    break;
                    
                case SDLK_RIGHT:
                    if(m_chrono.seconds()-lastTime >= .5f)
                    {
                        instridx = min(instridx+1, 3);
                        lastTime = m_chrono.seconds();
                    }
                    break;
                    
                case SDLK_BACKSPACE:
                    if(m_chrono.seconds()-lastTime >= .5f)
                    {
                        get_Game().pop_state();
                        lastTime = m_chrono.seconds();
                    }
                    break;
                    
                default:
                    Gamestate_Base::on_key(event); // Let Gamestate_Base handle it
                    break;
            }
        } else Gamestate_Base::on_key(event); // Let Gamestate_Base handle it
    }
    
    void render() {
        Widget_Gamestate::render();
        
        Point2f one = Point2f(0,0);
        Point2f two = one + Point2f(1024,1024);
        
        /*Zeni::Font &fr = get_Fonts()["title"];
         
         fr.render_text(
         #if defined(_WINDOWS)
         "ALT+F4"
         #elif defined(_MACOSX)
         "Apple+Q"
         #else
         "Ctrl+Q"
         #endif
         " to Quit",
         Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
         get_Colors()["title_text"],
         ZENI_CENTER);*/
        
        switch(instridx)
        {
            case 0:
                render_image("instr_1", one, two);
                break;
            case 1:
                render_image("instr_2", one, two);
                break;
            case 2:
                render_image("instr_3", one, two);
                break;
            case 3:
                render_image("instr_4", one, two);
                break;
        }
        
        //Zeni::Font &fd = get_Fonts()["instr"];
        //fd.render_text("DESTROY BLOCKS AND SURVIVE!\n\n\nControls\n------------\na/left = move left\nd/right = move right\nw/up = jump\nspace = destroy blocks\n\nWhile sliding on a wall, press the jump button to perform a wall jump.\n\nBreaking blocks to gain powerups!\n\n\n\nSong from user johnfn on newgrounds.com", Point2f(10, 10), get_Colors()["title_text"], ZENI_LEFT);
    }
};

class Title_State_Custom : public Title_State<Play_State, Instructions_State> {
public:
    Title_State_Custom()
    : Title_State<Play_State, Instructions_State>("")
    {
        m_widgets.unlend_Widget(title);
    }
    
    void render() {
        Title_State<Play_State, Instructions_State>::render();
        
        render_image("logo", Point2f(200.0f, 25.0f), Point2f(600.0f, 225.0f));
    }
};

class Bootstrap {
    class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
        virtual Gamestate_Base * operator()() {
            Window::set_title("Falling");
            
            get_Joysticks();
            get_Video();
            get_Textures();
            get_Fonts();
            get_Sounds();
            get_Game().joy_mouse.enabled = true;
            
            get_Sound().set_BGM("sfx/bkgr");
            get_Sound().set_BGM_looping(true);
            get_Sound().play_BGM();
            
            return new Title_State_Custom();
        }
    } m_goi;
    
public:
    Bootstrap() {
        g_gzi = &m_goi;
    }
} g_bootstrap;

int main(int argc, char **argv) {
    return zenilib_main(argc, argv);
}
