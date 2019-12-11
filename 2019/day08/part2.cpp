#include <string>
#include <iostream>
#include <vector>

#define WIDTH 25
#define HEIGHT 6

using namespace std;

struct Image {
    vector<vector<int>> pixels;

    Image(int width, int height) {
        pixels = vector<vector<int>>(height, vector<int>(width, 2));
    }

    void print() {
        for (auto& row : pixels) {
            for (int pixel : row) {
                cout << (pixel == 1 ? "1" : " ");
            }
            cout << endl;
        }
    }

    int digCount(int dig) {
        int count = 0;

        for (auto& row : pixels) {
            for (int pixel : row) {
                if (pixel == dig) {
                    count++;
                }
            }
        }

        return count;
    }
};

int main() {
    string input;
    cin >> input;

    int layerPixelCount = WIDTH * HEIGHT;
    int layerCount = input.length() / layerPixelCount;

    vector<Image> layers(layerCount, Image(WIDTH, HEIGHT));

    for (int i = 0; i < input.length(); i++) {
        int layer = i / layerPixelCount;
        int pixelNum = i - (layer * layerPixelCount);
        int row = pixelNum / WIDTH;
        int col = pixelNum % WIDTH;

        layers[layer].pixels[row][col] = input[i] - '0';
    }

    Image decode(WIDTH, HEIGHT);

    for (auto& layer : layers) {
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (decode.pixels[i][j] == 2) {
                    decode.pixels[i][j] = layer.pixels[i][j];
                }
            }
        }
    }

    decode.print();

    return 0;
}