#include "circle_generator_v04.h"

#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <string>
#include <fstream>
#include <QImage>
#include <filesystem>

/**
 * generates test data in form of circles with [n, m] number of pixels with noise
 * intended for generating data for RICH
 * m_rng is set within Circle_generator::loop_circle_generator
 * @author Tai Zhang
 * @version 1.0
*/
double Circle_generator::mt_rand_generator(double a1, double a2){
    std::uniform_real_distribution<double> dis(a1, a2);
    double rand_number = dis(m_rng);
    return rand_number;
};

void Circle_generator::canvas_drawer(){
    int size = final_canvas.canvas_size;
    final_canvas.canvas_array.assign(size, std::vector<int>(size, 0));
}

void Circle_generator::noise_generator(float a){
    try {
        if (0<a and a<1){
            for(int i=0;i<final_canvas.canvas_size;i++){
                for(int j=0;j<final_canvas.canvas_size;j++){
                    if (mt_rand_generator(0,1)>1-a){
                        final_canvas.final_list_of_coord.push_back(Point_Coordinates(i,j));
                    }
                }
            }
        }
        else{
            std::cerr << "Input value outside of interval [0, 1] in Circle_drawer/main.cpp, function noise_generator";
        }
    } catch (std::exception&e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

//writes circle with bresenham algorithm
void Circle_generator::bresenham_help(int x, int y, int p, int q){
    list_of_coord.push_back(Point_Coordinates(x+p, y+q));
    list_of_coord.push_back(Point_Coordinates(-x+p, y+q));
    list_of_coord.push_back(Point_Coordinates(x+p, -y+q));
    list_of_coord.push_back(Point_Coordinates(-x+p, -y+q));
    list_of_coord.push_back(Point_Coordinates(y+p, x+q));
    list_of_coord.push_back(Point_Coordinates(y+p, -x+q));
    list_of_coord.push_back(Point_Coordinates(-y+p, x+q));
    list_of_coord.push_back(Point_Coordinates(-y+p, -x+q));
}


void Circle_generator::circle_generator(int imin_dots, int imax_dots, int ir, int ip, int iq){
    int x, y, d;

    d = 3-2*ir;
    x = 0;
    y = ir;

    bresenham_help(x, y, ip, iq);

    while (x<y){
        if(d<=0){
            d=d+(4*x)+6;
            bresenham_help(++x, y, ip, iq);
        }
        else{
            d=d+(4*(x-y))+10;
            bresenham_help(++x, --y, ip, iq);
        }
    }

    //sort elements, then remove duplicate elements

    //generate a new list of random points between given [a, b]
    int rand_value = static_cast<int>(mt_rand_generator(imin_dots, imax_dots));

    // change in v03, calculating percentage based number of pixels
    int rand_num_pixels = list_of_coord.size()*rand_value/100;

    // shuffle and take the first rand_num_pixels elements from the list
    std::shuffle(list_of_coord.begin(), list_of_coord.end(), m_rng);
    for(int i = 0; i < rand_num_pixels && i < int(list_of_coord.size()); ++i) {
        final_canvas.final_list_of_coord.push_back(list_of_coord[i]);
    }

    list_of_coord.clear();
}

void Circle_generator::file_remover(std::string dir_path){
    for (const auto& entry : std::filesystem::directory_iterator(dir_path))
        std::filesystem::remove_all(entry.path());
}


/**
 * main looping function, number of loops, canvas size, min and max of pixels in circle, noise rate, radius
 * generate both positvee and negative samples
 * iloops = number of positive samples
 * inegloops = number of negative samples
 * isize = size of each sample, nxn
 * a percentage of pixels for each circle are actually drawn on the canvas randomly
 * imin_num_dots = lower bound of circle percentage
 * imax_num_dots = upper bound of circle percentage
 * inoise = random background noise for each sample
 * min_num_circles = minimum amount of circles of each radii
 * max_num_circles = maximum amount of circles of each radii
 * iradii = list of all radii
 * iradii_size = size of iradii
 *
 * ideally to use maxium space iradius = isize/4
 * recommend to set noise at 0.01-0.05
 * generates single file with each line a generated test data
 * generates both positive and negative samples
 *
 * file format: n, m_1, m_2, ..., m_n, data
 * n: number of lables, in our specific case 2
 * m_1, m_2, ..., m_n: individual lable values
 * data: rest of data in single line seperated by commas, for example a 30x30 field generates a single string of 900 zeroes/ones seperated by 899 commas
 */
void Circle_generator::loop_circle_generator(int iloops, int inegloops, int isize, int imin_num_dots, int imax_num_dots, float inoise, int min_num_circles,
                                             int max_num_circles, int* iradii, int iradii_size){
    // define size of canvas
    final_canvas.canvas_size = isize;

    // set mt_rnd_genertor
    std::random_device dev;
    m_rng.seed(dev());

    // output handling
    std::string data_path = output_directory;
    std::string data_name = output_name;

    // remove old content from output folder
    file_remover(data_path);

    // new file
    std::ofstream fw(data_path + data_name + "." + output_type, std::ofstream::out);

    // geneartes positive samples ##############################################################################################
    for (int i=0; i<iloops; i++){
        canvas_drawer();

        noise_generator(inoise);

        int* num_circle_array = (int*)calloc(iradii_size, sizeof(int));

        // generate a number between 0 and max_num_circles for the number of circles appearing on screen
        for (int j=0; j<iradii_size; j++){
            num_circle_array[j] = mt_rand_generator(min_num_circles, max_num_circles+1);
            // number of circles of different radii
            std::cout << std::to_string(num_circle_array[j]) + " ";
            std::cout << std::endl;

        }

        // list for future circle coord and radii size
        // r, x_coord, y_coord
        int total_circle_num = 0;
        for (int k=0; k<iradii_size; k++){
            total_circle_num += num_circle_array[k];
        }

        total_rad_coord_array = (int**)calloc(total_circle_num, sizeof(int*));
        for (int k=0; k<total_circle_num; k++){
            total_rad_coord_array[k] = (int*)calloc(3, sizeof(int));
        }

        int i_counter = 0;

        for (int r=0; r<iradii_size; r++){
            // draw the full circle inside the canvas
            int min_coord = iradii[r];
            int max_coord = isize-iradii[r];

            // draw circle
            for (int k=0; k<num_circle_array[r]; k++){
                //variance in circle location
                int x_coord = mt_rand_generator(min_coord, max_coord);
                int y_coord = mt_rand_generator(min_coord, max_coord);


                circle_generator(imin_num_dots, imax_num_dots, iradii[r], x_coord, y_coord);

                total_rad_coord_array[i_counter][0] = iradii[r];
                total_rad_coord_array[i_counter][1] = x_coord;
                total_rad_coord_array[i_counter][2] = y_coord;

                i_counter += 1;
            }
        }

        // ignore pixels outside of array range
        for(int k=0;k<static_cast<int>(final_canvas.final_list_of_coord.size());k++){
            if (final_canvas.final_list_of_coord[k].x<0 or final_canvas.canvas_size<=final_canvas.final_list_of_coord[k].x){
                continue;
            }
            if (final_canvas.final_list_of_coord[k].y<0 or final_canvas.canvas_size<=final_canvas.final_list_of_coord[k].y){
                continue;
            }

            final_canvas.canvas_array[final_canvas.final_list_of_coord[k].x][final_canvas.final_list_of_coord[k].y]=1.0;
        }

        if (visualization_status){
            for(int i=0; i<final_canvas.canvas_size; i++){
                for(int j=0; j<final_canvas.canvas_size; j++){
                    std::cout << final_canvas.canvas_array[i][j];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        };

        if (fw.is_open())
        {
            // store array contents to file
            std::string final_string;

            // skip for 0 circles
            if (total_circle_num != 0){
                // add number of circles into string
                final_string += std::to_string(total_circle_num) + ",";

                // add radii + x_coord + y_coord into string
                for (int m=0; m<total_circle_num; m++){
                    for (int n=0; n<3; n++){
                        final_string += std::to_string(int(total_rad_coord_array[m][n])) + ",";
                    }
                }
            } else {
                final_string += std::to_string(-1) + ",";
            }

            for (int m=0;m<final_canvas.canvas_size;m++) {
                for (int n=0;n<final_canvas.canvas_size;n++){
                    final_string += std::to_string(final_canvas.canvas_array[m][n]);
                    final_string += ",";
                }
            }
            final_string += "\n";
            fw << final_string;
        }
        else std::cout << "Problem with opening file" << std::endl;

        list_of_coord.clear();
        final_canvas.final_list_of_coord.clear();


        if (i%100==0){
            std::cout << "Writing positive sample file Nr. " << i << std::endl;
        }

        free(total_rad_coord_array);

        free(num_circle_array);
    }

    // generate negative samples #####################################################################
    std::cout << "Generating negative samples" << std::endl;
    for (int i=0; i<inegloops; i++){
        canvas_drawer();

        noise_generator(inoise);

        for(int k=0;k<static_cast<int>(final_canvas.final_list_of_coord.size());k++){
            final_canvas.canvas_array[final_canvas.final_list_of_coord[k].x][final_canvas.final_list_of_coord[k].y]=1.0;
        }

        if (visualization_status){
            for(int i=0; i<final_canvas.canvas_size; i++){
                for(int j=0; j<final_canvas.canvas_size; j++){
                    std::cout << final_canvas.canvas_array[i][j];
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        };

        if (fw.is_open())
        {
            // store array contents to file
            std::string final_string;

            // add correct labeling to final_string
            final_string += std::to_string(-1) + ",";
//            final_string += std::to_string(iradii_size) + ",";
//            for(int l=0; l<iradii_size; l++){
//                final_string += "0,";
//            }

            // add all content of final_canvas to string
            for (int m=0;m<final_canvas.canvas_size;m++) {
                for (int n=0;n<final_canvas.canvas_size;n++){
                    final_string += std::to_string(final_canvas.canvas_array[m][n]);
                    final_string += ",";
                }
                // decomment the following three comments for better visibilities of circles in test data
//                temp_string += "\n";
            }
            final_string += "\n";
            fw << final_string;
        }
        else std::cout << "Problem with opening file" << std::endl;
        list_of_coord.clear();
        final_canvas.final_list_of_coord.clear();


        if (i%100==0){
            std::cout << "Writing negative sample file Nr. " << i << std::endl;
        }

//        external_file_generator(i, data_path, "Test_Circles_neg_", output_type);
    }

    fw.close();
}
