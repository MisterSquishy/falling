/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include "BlockHandler.h"

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
  }

  ~Play_State()
  {
  }

  void on_key(const SDL_KeyboardEvent &event) {
    switch(event.keysym.sym) {
      case SDLK_w:
			cs.jump = event.type == SDL_KEYDOWN;
        break;
 
      case SDLK_a:
			cs.left = event.type == SDL_KEYDOWN;
        break;
 
      case SDLK_s:
			//m_backward = event.type == SDL_KEYDOWN;
        break;
 
      case SDLK_d:
			cs.right = event.type == SDL_KEYDOWN;
        break;
 
      default:
        Gamestate_Base::on_key(event); // Let Gamestate_Base handle it
        break;
    }
  }

private:

  Color m_prev_clear_color;

  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    //get_Game().joy_mouse.enabled = false;

	get_Video().set_clear_Color(Color(1.0f, 0.0f, 150.0f, 193.0f));
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
    const float time_passed = m_chrono.seconds();
    const float time_step = time_passed - m_time_passed;
    m_time_passed = time_passed;

	bh.perform_logic(time_passed, time_step, cs);
  }

  void render() {
    get_Video().set_2d();
 
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
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["title"];

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
                   ZENI_CENTER);
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

      return new Title_State<Play_State, Instructions_State>("Zenipex Library\nApplication");
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
