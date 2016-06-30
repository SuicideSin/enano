#include <ncurses.h>
#include <functional>
#include <string>
#include <vector>

class editor_t
{
	public:
		editor_t();

		void start(const std::string& filename,const std::string& data);
		void stop();

		void type_lines(const std::vector<std::string>& lines);
		void type_string(const std::string& str);

		void home();
		void end();

		void move_left();
		void move_right();
		void move_up();
		void move_down();

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
		bool stop_m;
		int yoff_m;
		int xoff_m;
		std::string name_m;
		std::vector<std::string> lines_m;
};