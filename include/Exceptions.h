#pragma once
#include <exception>
#include <string>

class ColisNotFoundException : public std::exception {
private:
    std::string message;
public:
    explicit ColisNotFoundException(int id) {
        message = "Erreur : le colis avec l'ID " + std::to_string(id) + " est introuvable.";
    }
    const char* what() const noexcept override { return message.c_str(); }
};

class InvalidStateTransitionException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Erreur : Transition d'etat invalide pour ce colis (Retour en arriere interdit) !";
    }
};
