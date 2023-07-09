#include <ncurses.h>
#include <unistd.h>
#include <chrono>
#include <cstring>

using namespace std;

const int FPS = 12;
const int FPS_MULTIPLIER = 60 / FPS;

class Screen
{
    private:
        char buffer[20][80];
        const int SCREEN_WIDTH = 80;
        const int SCREEN_HEIGHT = 20;
        int frame = 0;


    public:
        //Screen()
        //{
        //    buffer
        //}

        /*
         * Take an input 2d array of size 20x80 and add a border to the outsides.
         */
        void add_border()
        {
            // First row
            buffer[0][0] = '*';
            for (int col = SCREEN_WIDTH - 3; col >= 0; --col)
            {
                buffer[0][col + 1] = '-';
            }
            buffer[0][SCREEN_WIDTH - 1] = '*';

            // Middle 17 rows
            for (int row = SCREEN_HEIGHT - 2; row >= 1; --row)
            {
                buffer[row][0] = '|';
                /*for (int col = SCREEN_WIDTH - 2; col >= 1; --col)
                {
                    board[row][col] = ' '; // Can set inside row/col here
                }*/
                buffer[row][SCREEN_WIDTH - 1] = '|';
            }

            // Bottom row
            buffer[SCREEN_HEIGHT - 1][0] = '*';
            for (int col = SCREEN_WIDTH - 3; col >= 0; --col)
            {
                buffer[SCREEN_HEIGHT - 1][col + 1] = '-';
            }
            buffer[SCREEN_HEIGHT - 1][SCREEN_WIDTH - 1] = '*';
        }

        /*
         * Take an input buffer and send it to the screen
         */
        void draw_screen_buffer()
        {
            for (int row = SCREEN_HEIGHT - 1; row >= 0; --row) // Draw the entire buffer to the screen
            {
                for (int col = SCREEN_WIDTH - 1; col >= 0; --col)
                {
                    mvaddch(row, col, buffer[row][col]);
                }
            }
            ++frame;
        }

        /*
         * Take an input buffer and update it ( do your drawing)
         */
        void update_buffer()
        {
            char chars[] = {'*', '&', '^', '%', '$'};

            int animate = frame % 4;

            for (int row = SCREEN_HEIGHT - 2; row >= 1; --row)
            {
                for (int col = SCREEN_WIDTH - 2; col >= 1; --col)
                {
                    buffer[row][col] = chars[col % (animate + 1)];
                }
                if (animate < 4)
                {
                    ++animate;
                }
                else
                {
                    animate = 0;
                }
            }
        }

        /*
         * Add an fps and frame counter to the screen
         */
        void show_fps(std::chrono::_V2::steady_clock::time_point frame_start_time, std::chrono::_V2::steady_clock::time_point frame_end_time)
        {
            //using milliseconds = std::chrono::duration<double, std::milli>;
            using microseconds = std::chrono::duration<double, std::micro>;
            char st[6]; // 6 length string, can hold up to 999,999 frames ~2 days
            sprintf(st, "%d", frame);
            mvaddstr(30, 29, "Frames: ");
            mvaddstr(30, 37, st);

            microseconds frame_time_mseconds = frame_end_time - frame_start_time;
            double frame_time = frame_time_mseconds.count();
            char ft[15];
            sprintf(ft, "%f", -1 / (frame_time));
            mvaddstr(31, 29, "FPS: ");
            mvaddstr(31, 37, ft);
        }

        /*
         * Holds the game loop
         */
        void game_loop(bool debug)
        {
            std::chrono::steady_clock::time_point frame_start_time = std::chrono::steady_clock::now();
            std::chrono::steady_clock::time_point frame_end_time = std::chrono::steady_clock::now();
            // Game logic loop
            while (true)
            {
                frame_start_time = std::chrono::steady_clock::now();

                // Draw on the buffer
                update_buffer();

                // Add the border
                add_border();

                // Draw the buffer onto the actual screen
                draw_screen_buffer();
                
                if (debug)
                {
                    // Draw the fps on the screen
                    show_fps(frame_start_time, frame_end_time);
                }

                // Move cursor off the frame
                move(30, 100);

                // Refresh the screen
                refresh();

                // pause for enough microseconds to be 6fps (change second number to be divisor of 60)
                usleep(16667 * FPS_MULTIPLIER);
                frame_end_time = std::chrono::steady_clock::now();
            }
        }
};



int main(int argc, char ** argv)
{
    bool debug = false;

    // Check command line args
    if (argc > 1)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (strcmp(argv[i], "-debug") == 0) // If we want debug info on screen
            {
                debug = true;
            }
        }
    }
    
    // init screen and sets up screen
    initscr();

    // New screen object
    Screen s;

    // Start the game loop
    s.game_loop(debug);

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}
