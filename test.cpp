#include <ncurses.h>
#include <unistd.h>

using namespace std;

const int FPS = 6;
const int FPS_MULTIPLIER = 60 / FPS;

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
            for (int col = SCREEN_WIDTH - 3; col >= 0; --col)
            {
                buffer[0][col+1] = '-';
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
            for (int col = SCREEN_WIDTH - 3; col >= 0; --col)
            {
                buffer[SCREEN_HEIGHT - 1][col+1] = '-';
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

    // Game logic loop
    while (true)
    {
        // Draw on the buffer
        s.update_buffer(frame);

        // Add the border
        s.add_border();

        // Draw the buffer onto the actual screen
        s.draw_screen_buffer();
        // Move cursor off the frame
        move(30, 100);
        // Refresh the screen
        refresh();

        ++frame;

        // pause for enough microseconds to be 6fps (change second number to be divisor of 60)
        usleep(16667 * FPS_MULTIPLIER);
    }

    // deallocates memory and ends ncurses
    endwin();
    return 0;
}
