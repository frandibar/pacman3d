#pragma once

/**
 * Generador de numeros aleatorios
 * Clase Service (no se instancia. Todos sus metodos son de clase)
 */
class RandomGenerator 
{
public:
    /// entre 0 y RANDOM_MAX 
    static int get();
    
    /// flotante entre 0 y 1
    static double getf();
    
    /// entre rango
    static int getRange(int min, int max);

private:
    static const int RANDOM_MAX;
    RandomGenerator();
};

