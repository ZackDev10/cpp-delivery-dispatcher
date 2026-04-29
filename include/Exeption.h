#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

#include <string>


class colisNOTFOUNDEXCEPTION : public std::exception { 
    private :
    std::string message ;
    public :
    colisNOTFOUNDEXCEPTION (int id){
        message = "Erreue : le colis avec l'ID "+ std::to_string(id) + "est introuvable .";
    };

    const char* what() const noexcept override {
        return message.c_str();
    };
    

} ;

class InvalidStateTransitionException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Erreur : Transition d'etat invalide pour ce colis !";
    }
};

#endif

