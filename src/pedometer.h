#ifndef PEDOMETER_H
#define PEDOMETER_H


class Pedometer {
    public:
        Pedometer(); //default constructor
        Pedometer(int xPin_, int yPin_, int zPin_); //custom pin constructor
        int StepAlgorithm (int x, int y, int z, int stepsCount);
        void Pedometer::getAxisData(int& x, int& y, int& z);
    private:
        int xPin_, yPin_, zPin_;
        
};

#endif //PEDOMETER_H