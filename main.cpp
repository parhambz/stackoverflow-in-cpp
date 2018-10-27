#include <iostream>
#include <vector>
#include <exception>
using namespace std;

/**
 * In the name of God
 * Homework 2
 * feel free to edit code as you want and make it better
 * Any questions? ask @devcom_support on telegram
 * Good luck!
 **/

enum UserType{
    ADMIN,
    MEMBER
};


class UserAlreadyExistsException:public exception{
public:
    const char * what () const throw(){
        return "User Already Exists";
    }
};
class UsernameOrPasswordWrongException:public exception {
public:
    const char * what()const throw(){
        return "username or password is wrong";
    }
};
class AbstractUser{ // User structure
public:
    string username;
    virtual bool authenticate(string username, string password) = 0;
    virtual bool deleteAccount(vector<AbstractUser*> *users) = 0;
    virtual string getUsername()=0;
protected:
    string password;
    UserType type;
};


class User : public AbstractUser{
public:
    string getUsername(){
        return this->username;
    }
    string username;
    User(string username, string password, UserType type){
        this->username = username;
        this->password = password;
        this->type = type;
    }

    bool authenticate(string username, string password){
        return this->username == username && this->password == password;
    }

    static User* login(vector<AbstractUser*> *users, string username, string password){
        for(auto user = users->begin(); user != users->end(); user++){
            if((*user)->authenticate(username, password)){
                return (User*) *user;
            }
        }
        UsernameOrPasswordWrongException ex;
        throw ex;
    }

    static void signup(vector<AbstractUser*> *users, string username, string password){

        //Check if user with that username exists and throw UserAlreadyExistsException in that case
        for(auto user = users->begin(); user != users->end(); user++) {
            if ((*user)->getUsername() == username) {
                UserAlreadyExistsException ex;
                throw ex;
            }
        }

        //Create user and add it to vector
        users->push_back(new User(username, password, UserType::MEMBER));
    }
    bool deleteAccount(vector<AbstractUser*> *users){

        for(auto user = users->begin(); user != users->end(); user++) {
            if ((*user)->getUsername() == this->username) {
                users->erase(user);
                return true;
            }

        }
        return false;
    }

};

enum MenuState{
    START,
    LOGGED_IN,
    END
};

class AppDatabase { //Just holds runtime data. doesn't save anything
public:
    vector<AbstractUser *> appUsers;

    AppDatabase() { //Load initial data
        appUsers.push_back(new User("admin",
                                    "admin" /* password is unsafe! for test only */,
                                    UserType::ADMIN)
        );
    }
};

int main(){
    User * loggedInUser = nullptr;
    AppDatabase appDatabase;
    MenuState menuState = MenuState::START;

    char choice;
    cout << "Welcome!" << endl;

    while(menuState != MenuState::END){
        switch (menuState){
            case MenuState::START: {

                cout << "1. login\n2. signup\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case '1': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;

                        try{
                            loggedInUser = User::login(&appDatabase.appUsers, username, password);
                            menuState = MenuState::LOGGED_IN;
                        }
                        catch (UsernameOrPasswordWrongException ex){
                            cout<<ex.what()<<endl;
                        }
                        break;
                    }
                    case '2': {
                        string username, password;
                        cout << "Enter Username" << endl;
                        cin >> username;
                        cout << "Enter Password" << endl;
                        cin >> password;
                        try{
                            User::signup(&appDatabase.appUsers, username, password);
                        } catch (UserAlreadyExistsException e) {
                            cout << "Error: username already exists";
                        }
                        break;
                    }
                    case 'e': {
                        menuState = MenuState::END;
                        break;
                    }
                    default: {
                        cout << "Unknown Input" << endl;
                    }
                }
                break;
            }
            case MenuState::LOGGED_IN: {
                cout << "d.delete account\nl. logout\ne. exit\n";
                cin >> choice;
                switch(choice) {
                    case 'd': {
                        cout<<loggedInUser->deleteAccount(&appDatabase.appUsers);
                        cout << "Account successfully deleted";
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'l': {
                        loggedInUser = nullptr;
                        menuState = MenuState::START;
                        break;
                    }
                    case 'e': {
                        menuState = MenuState::END;
                        break;
                    }
                    default: {
                        cout << "Unknown Input" << endl;
                    }
                }
                break;
            }
        }
    }

    return 0;

}

