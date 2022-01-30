#include <tpf_output.hpp>

tpf::sstream stream;
auto endl = tpf::endl;

/*
    https://edotor.net/

    graph Animal
    {
        Animal -- Canis;
        Animal -- Bird;
        Canis -- Wolf;
        Canis -- Dog;

        Dog -- JindoDog;
        Dog -- ChowChow;

        Bird -- Sparrow;
        Bird -- Eagle;
        Bird -- Parrot;

    }
 */

class Animal
{
    public:

    // once we declare virtual abstract member function
    // in the base class, all inherited class should define
    // its body construct
    virtual void species() = 0;
    virtual void speak() = 0;

    // this virtual destructor
    // in the base class of the inheritance chain
    // makes all inherited classes create a default virtual destructor
    virtual ~Animal() = default;
};

class Canis: public Animal
{
    public:

    virtual void bark() = 0;
    
    void species() override
    { 
        stream <<" Species: Canis ";
    }

};

class Wolf: public Canis
{
    public:
    
    void bark() override
    {
        stream << "Wolf: howling howling "<< endl;
    }

    void speak() override
    {
        stream << " I am a wolf ";
        bark();
    }
};

class Dog: public Canis
{
    public:

    void bark() override
    {
        stream << " Dog: baw baw baw bow"<< endl;
    }

    void speak() override
    {
        stream << "I am a dog."<<endl;
        bark();
    }
};

class JindoDog: public Dog
{
    public:

    void bark() override
    {
        stream << " JindoDog: jindo jindo" << endl;
    }

    void speak() override
    {
        stream << " I am a dog from Jindo, Korea " << endl;
        bark();
    }
};

class ChowChow: public Dog
{
    public:

    void bark() override
    {
        stream << " Chow Chow: chow chow" << endl;
    }

    void speak() override
    {
        stream << " I am a dog from China " << endl;
        bark();
    }

};

class Bird: public Animal
{
    public:

    virtual void chirp() = 0;

    void species() override
    { 
        stream <<" Species: Bird ";
    }

    void speak() override
    {
        stream << "I am a bird. " << endl;

        chirp();
    }

};

class Sparrow: public Bird
{
    public:

    void chirp() override
    {
        stream << "Sparrow chirping: sparrow sparrow" << endl;
    }
};

class Eagle: public Bird
{
    public:

    void chirp() override
    {
        stream << " Eagle: chirping eagle eagle " << endl;
    }

};

class Parrot: public Bird
{
    public:

    void chirp() override
    {
        stream << " Parrot: singing parrow parrow " << endl;
    }
};

void animal_speak(Animal& my_pet)
{
    my_pet.speak();
}

void canis_bark(Canis& my_canis)
{
    my_canis.bark();
}

void bird_chirp(Bird& my_bird)
{
    my_bird.chirp();
}

void test_animals()
{
    Wolf w;
    JindoDog j;
    ChowChow c;
    Sparrow s;
    Eagle e;
    Parrot p;

    stream <<"Canis can bark"<< endl << endl;
    canis_bark(w);
    canis_bark(j);
    canis_bark(c);

    stream <<"\nBirds can chirp"<< endl << endl;
    bird_chirp(e);
    bird_chirp(p);

    stream <<"\nAnimals can speak" << endl << endl;
    animal_speak(w);
    animal_speak(j);
    animal_speak(c);
    animal_speak(s);
    animal_speak(e);
    animal_speak(p);
}

int main()
{
    test_animals();
}