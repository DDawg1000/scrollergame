#include <ncurses.h>
#include <unistd.h>

using namespace std;

class Screen
{
    private:
        char buffer[20][80];
        const int SCREEN_WIDTH = 80;
        const int SCREEN_HEIGHT = 20;

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
            for (int i = SCREEN_WIDTH - 3; i >= 0; --i)
            {
                buffer[0][i+1] = '-';
            }
            buffer[0][SCREEN_WIDTH - 1] = '*';

            // Middle 17 rows
            for (int row = SCREEN_HEIGHT - 2; row >= 1; --row)
            {
                buffer[row][0] = '|';
                for (int col = SCREEN_WIDTH - 2; col >= 1; --col)
                {
                    //board[row][col] = ' '; // Can set inside row/col here
                }
                buffer[row][SCREEN_WIDTH - 1] = '|';
            }

            // Bottom row
            buffer[SCREEN_HEIGHT - 1][0] = '*';
            for (int i = SCREEN_WIDTH - 3; i >= 0; --i)
            {
                buffer[SCREEN_HEIGHT - 1][i+1] = '-';
            }
            buffer[SCREEN_HEIGHT - 1][SCREEN_WIDTH - 1] = '*';
        }

        /*
        * Take an input buffer and send it to the screen
        */
        void draw_screen_buffer()
        {
            for (int i = SCREEN_HEIGHT - 1; i >= 0; --i) // Draw the entire buffer to the screen
            {
                for (int j = SCREEN_WIDTH - 1; j >= 0; --j)
                {
                    mvaddch(i, j, buffer[i][j]);
                }
            }
        }

        /*
        * Take an input buffer and update it ( do your drawing)
        */
        void update_buffer(int animate)
        {
            char chars[] = {'*', '&', '^', '%', '$'};

            animate = animate % 4;

            for (int i = SCREEN_HEIGHT - 2; i >= 1; --i)
            {
                for (int j = SCREEN_WIDTH - 2; j >= 1; --j)
                {
                    buffer[i][j] = chars[j % (animate + 1)];
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
};

int main(int argc, char ** argv)
{
    // init screen and sets up screen
    initscr();

    Screen s;
    
    int frame = 0;

    while (true)
    {
        // Draw on the buffer
        s.update_buffer(frame);

        // Add the border
        s.add_border();

        // refreshes the screen
        s.draw_screen_buffer();
        refresh();

        ++frame;

        // pause for enough microseconds to be 6fps (change second number to be divisor of 60)
        usleep(16667 * 10);
    }

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}
