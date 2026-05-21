#pragma once

#include <vector>
#include <functional>

class ErrorEvaluation
{
protected:
    double epsilon;

public:
    ErrorEvaluation(double epsilon_) : epsilon(epsilon_) {}
    virtual ~ErrorEvaluation() {}

    // Implement this in derived classes

    // void cacheOld(double old_value, double x, double y) { ... }
    // void evaluateNew(double new_value, double x, double y) { ... }

    // // Should calculations continue or not
    // bool getEvaluationResult() { ... }
};


class NoErrorEvaluation final : public ErrorEvaluation
{
public:
    NoErrorEvaluation(double epsilon_) : ErrorEvaluation(epsilon_) {}
    virtual ~NoErrorEvaluation() {}

    void cacheOld(double old_value, double x, double y) {}
    void evaluateNew(double new_value, double x, double y) {}

    // Should calculations continue or not
    bool getEvaluationResult() { return true; }
};


class AnalyticalErrorEvaluation final : public ErrorEvaluation
{
    std::function<double(double, double)> analytical;
    
    double maxError;

public:
    AnalyticalErrorEvaluation(double epsilon_, std::function<double(double, double)> analyticalSolution) : 
        ErrorEvaluation(epsilon_), analytical(analyticalSolution) {}
    virtual ~AnalyticalErrorEvaluation() {}

    void cacheOld(double old_value, double x, double y) {}

    void evaluateNew(double new_value, double x, double y) 
    { 
        double error = abs(analytical(x, y) - new_value);
        if (error > maxError)
            maxError = error;
    }

    // Should calculations continue or not
    bool getEvaluationResult() 
    { 
        bool result =  maxError > epsilon; 
        maxError = 0.0;
        return result;
    }
};


class HeuristicErrorEvaluation : public ErrorEvaluation
{
    double cachedOld = 0.0;
    double maxError = 0.0;

public:
    HeuristicErrorEvaluation(double epsilon_) : 
        ErrorEvaluation(epsilon_) {}
    virtual ~HeuristicErrorEvaluation() {}

    // Implement this in derived classes

    void cacheOld(double old_value, double x, double y) 
    {
        cachedOld = old_value;
    }

    void evaluateNew(double new_value, double x, double y) 
    {
        double delta = abs(new_value - cachedOld);
        if (delta > maxError)
            maxError = delta;
    }

    // Should calculations continue or not
    bool getEvaluationResult() 
    { 
        bool result = maxError > epsilon; 
        maxError = 0.0;
        return result;
    }
};
