// my first program in C++
#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <iostream>
#include <fstream>

class Trajectory {
    std::vector<double> x;
    std::vector<double> y;
public:
    Trajectory(std::vector<double> x, std::vector<double> y) : x(x), y(y) {};
    Trajectory() {
        x.clear();
        y.clear();
        x.push_back(0);
        y.push_back(0);
    }

    std::vector<double> gety() {
        return y;
    }
    std::vector<double> getx() {
        return x;
    }
};

class Generator {
    double n;
    std::vector<double> uni;
    std::vector<double> norm;
public:
    Generator(double n) :n(n) {};
    std::vector<double> Box_Muller(std::vector<double> x){
        uni.clear();
        norm.clear();
        for (int i = 0; i <= n - 1; i++)
        {
            double a = (double)rand() / RAND_MAX;
            if (a < 0.000001) {
                i--;
            }
            else {
                uni.push_back(a);
            }
        }
        for (int j = 0; j <= n - 1; j += 2) {
            double dt = x[j+1] - x[j];
            double u1 = uni[j];
            double u2 = uni[j + 1];
            double t = 2 * M_PI * u2;
            double r = sqrt(dt)*sqrt(-2.0 * log(u1));
            norm.push_back(r * cos(t));
            norm.push_back(r * sin(t));
        }
        return norm;
    }

    std::vector<double> discretization(double n) {
        double T = 1;
        double a = 0;
        double da = T / double(n);
        std::vector<double> x;
        for (int i = 0; i <= n; i++) {
            x.push_back(a);
            a += da;
        }
        return x;
    }
    Trajectory one_trajectory(std::vector<double> x, Trajectory &T) {
        std::vector<double> increments = Box_Muller(x);
        double B0 = 0;
        std::vector<double> y;
        for (unsigned int i = 0; i < increments.size(); i++) {
            y.push_back(B0);
            B0 += increments[i];
        }
        T = Trajectory(x, y);
        return T;
    }

    bool nacitanie_do_dsv(double Ntrajectory, std::vector<Trajectory> all_trajectories) {
        std::fstream textout;
        int True = 0, False = 0;
        textout.open("Trajectories.dsv", std::ios::out);
        if (textout.is_open()) {
            textout << "aes_x";
            textout << "|aes_y" << "|value" << "|color";
            textout << std::endl;

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < Ntrajectory; j++) {
                    textout << all_trajectories[j].getx()[i];
                    textout << "|y" << j;
                    textout << "|" << all_trajectories[j].gety()[i];
                    if (all_trajectories[j].gety()[1] > 0. && all_trajectories[j].gety()[2] > all_trajectories[j].gety()[1] && all_trajectories[j].gety()[3] > all_trajectories[j].gety()[2]) {
                        textout << "|1";
                        True++;
                    }
                    else {
                        textout << "|0";
                        False++;
                    }
                    textout << std::endl;
                }
            }
            textout.close();
            std::cout << "zhoduje sa" << True / (double)(True + False) << std::endl;
            return true;
        }
        else {
            std::cout << "neotvorilo subor";
            return false;
        }
        
    }
};


int main()
{
    srand((unsigned)time(NULL));
    double n = 100;
    double Ntrajectory = 10000;
    Generator N(n);
    std::vector<double> x = N.discretization(n);
    std::vector<Trajectory> all_trajectories;
    for (int i = 0; i < Ntrajectory; i++) {
        Trajectory T1;
        N.one_trajectory(x, T1);
        all_trajectories.push_back(T1);
    }
    N.nacitanie_do_dsv(Ntrajectory, all_trajectories);
}