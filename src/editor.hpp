//Dependencies:
//		-lncurses

#include <ncurses.h>

#include <functional>
#include <string>
#include <vector>

class editor_t
{
	public:
		editor_t();

		void start(const std::string& filename,const std::string& data,std::function<bool(const std::string&)> save_func);
		void stop();

		void type_lines(const std::vector<std::string>& lines);
		void type_string(const std::string& str);

		void home();
		void end();

		void move_left();
		void move_right();
		void move_up(bool page=false);
		void move_down(bool page=false);

		void newline();
		void backspace();
		void del();

		void insert_char(const char ch);

		void draw_top_bar();
		void draw_bottom_bar();
		void resize();

		void get_pos(int& y,int& x);
		void move_pos(const int y,const int x);
		int max_y();

	private:
		bool refresh_m;
		bool stop_m;
		std::function<bool(const std::string&)> save_func_m;
		int y_top_margin_m;
		int y_bottom_margin_m;
		int yoff_m;
		int xoff_m;
		std::string name_m;
		std::vector<std::string> lines_m;
		std::string status_m;
};
