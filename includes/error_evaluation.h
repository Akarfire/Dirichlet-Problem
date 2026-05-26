#pragma once

#include <vector>
#include <functional>

class ErrorEvaluation
{
public:
    ErrorEvaluation() {}
    virtual ~ErrorEvaluation() {}

    // Implement this in derived classes

    // void setupErrorEvaluation(...) { ... } (optional)

    // inline void cacheOld(double old_value, double x, double y) { ... }
    // inline void evaluateNew(double new_value, double x, double y) { ... }

    // // Should calculations continue or not
    // inline bool getEvaluationResult() { ... }
};


class NoErrorEvaluation : public ErrorEvaluation
{
public:
    NoErrorEvaluation() : ErrorEvaluation() {}
    virtual ~NoErrorEvaluation() {}

    inline void cacheOld(double old_value, double x, double y) {}
    inline void evaluateNew(double new_value, double x, double y) {}

    // Should calculations continue or not
    inline bool getEvaluationResult() { return true; }
};


class AnalyticalErrorEvaluation : public ErrorEvaluation
{
protected:
    double epsilon = 0.0;
    std::function<double(double, double)> analytical;
    double maxError;

    double lastStepError = 0.0;

public:
    AnalyticalErrorEvaluation() : ErrorEvaluation() {}
    virtual ~AnalyticalErrorEvaluation() {}

    void setupErrorEvaluation(double epsilon_, std::function<double(double, double)> analyticalSolution)
    {
        epsilon = epsilon_;
        analytical = analyticalSolution;
    }

    inline void cacheOld(double old_value, double x, double y) {}

    inline void evaluateNew(double new_value, double x, double y) 
    { 
        double error = abs(analytical(x, y) - new_value);
        if (error > maxError)
            maxError = error;
    }

    // Should calculations continue or not
    inline bool getEvaluationResult() 
    { 
        bool result =  maxError > epsilon;
        lastStepError = maxError;
        maxError = 0.0;
        return result;
    }

    inline double getLastStepError() { return lastStepError; }
};


class HeuristicErrorEvaluation : public ErrorEvaluation
{
protected:
    double epsilon = 0.0;

    double cachedOld = 0.0;
    double maxError = 0.0;

    double lastStepError = 0.0;

public:
    HeuristicErrorEvaluation() : ErrorEvaluation() {}
    virtual ~HeuristicErrorEvaluation() {}

    void setupErrorEvaluation(double epsilon_)
    {
        epsilon = epsilon_;
    }

    inline void cacheOld(double old_value, double x, double y) 
    {
        cachedOld = old_value;
    }

    inline void evaluateNew(double new_value, double x, double y) 
    {
        double delta = abs(new_value - cachedOld);
        if (delta > maxError)
            maxError = delta;
    }

    // Should calculations continue or not
    inline bool getEvaluationResult() 
    { 
        bool result = maxError > epsilon; 
        lastStepError = maxError;
        maxError = 0.0;
        return result;
    }

    inline double getLastStepError() { return lastStepError; }
};
