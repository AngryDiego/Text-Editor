#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#define stopCase break

class Document {
    std::string documentName;
    std::vector <std::string> lines;

public :
    void AddLine(std::string text) {
        lines.push_back(text);
    }

    void DeleteLine(int line) {
        line--;
        if (line > lines.size() || line < 0) { return; }
        else {
            lines.erase(lines.begin() + line);
        }
    }

    void Print() {
        int index = 0;

        for (auto& line : lines) {
            std::cout << index + 1 << " : " << line << std::endl;
            index++;
        }
    }

    void LoadFromFile (std::string fName) { //Without .TxT extention
        std::ifstream froamFile (fName + ".txt");
        if (!froamFile) { std::cout << "404 File not found"; return; }
        std::string line;
        lines.clear();
        while (std::getline(froamFile, line)) {
            lines.push_back(line);
        }
    }

    void SaveToFile (std::string fName) { //Without .TxT extention
        std::ofstream toFile(fName + ".txt");
        for (auto& line : lines) {
            toFile << line << std::endl;
        }
    }

    Document Clone() { return *this; }
};

class CommandManager {
    std::stack <Document> undoStack;
    std::stack <Document> redoStack;

public:
    void SaveState(Document& doc) {
        undoStack.push(doc.Clone());
        while (!redoStack.empty()) {
            redoStack.pop();
        }
    }
    bool CanUndo() { return !undoStack.empty(); }
    bool CanRedo() { return !redoStack.empty(); }

    Document Undo(Document& current) {
        if (CanUndo()) {
            redoStack.push(current.Clone());
            Document previous = undoStack.top();
            undoStack.pop();
            return previous;
        }
        std::cout << "No actions to undo " << std::endl;
        return current;
    }

    Document Redo(Document& current) {
        if (CanRedo()) {
            undoStack.push(current.Clone());
            Document next = redoStack.top();
            redoStack.pop();
            return next;
        }
        std::cout << "No actions to redo " << std::endl;
        return current;
    }
};

void ShowMenu() {
    std::cout << "1.Create new document" << std::endl;

    std::cout << "2.Open document" << std::endl;

    std::cout << "3.Print document" << std::endl;

    std::cout << "4.Add line" << std::endl;

    std::cout << "5.Delete line" << std::endl;

    std::cout << "6.Save to File" << std::endl;

    std::cout << "7.Un do" << std::endl;

    std::cout << "8.Re do" << std::endl;

    std::cout << "0.Exit (you should kill yourself)" << std::endl;
}

void Ask(std::string choice) {
    
}

int main () {
    Document doc;

    CommandManager cmdm;

    int choice = 10;

    int answer = 0;

    while (choice != 0) {
        ShowMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            std::cout << "Do you want to create new document? \n1.Yes \n2.No" << std::endl;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 2) { break;}
            else { continue; }

            doc = Document();
            std::cout << "New doc created" << std::endl;
            break;

        case 2: {
            std::cout << "Do you want to Load from file? \n1.Yes \n2.No" << std::endl;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 2) { break; }
            else { continue; }

            std::string fname;
            std::cout << "Enter file name to load from (Without .txt extention) : ";
            std::getline(std::cin, fname);
            doc.LoadFromFile(fname);
            break;
        }

        case 3:
            doc.Print();
            break;

        case 4: {
            std::cout << "Do you want to add new line? \n1.Yes \n2.No" << std::endl;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 2) { break; }
            else { continue; }

              std::string newline;
              std::cout << "Enter new line : ";
              std::getline(std::cin, newline);
              cmdm.SaveState(doc);
              doc.AddLine(newline);
              break;
        }

        case 5: {
            std::cout << "Do you want to delete line? \n1.Yes \n2.No" << std::endl;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 2) { break; }
            else { continue; }

            int delline;
            std::cout << "Enter which line you want to delete : ";
            std::cin >> delline;
            std::cin.ignore();
            cmdm.SaveState(doc);
            doc.DeleteLine(delline);
            break;
        }

        case 6: {
            std::cout << "Do you want to save to file? \n1.Yes \n2.No" << std::endl;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 2) { break; }
            else { continue; }

            std::string fname;
            std::cout << "Enter name of file to save (Without .txt extention) :";
            std::getline(std::cin, fname);
            doc.SaveToFile(fname);
            break;
        }

        case 7:
            doc = cmdm.Undo(doc);
            break;

        case 8:
            doc = cmdm.Redo(doc);
            break;

        case 0: 
            std::cout << "Do you want to exit? \n1.Yes \n2.No" << std::endl;
            std::cin >> answer;
            std::cin.ignore();
            if (answer == 2) { break; }
            else { continue; }

            std::exit(0);
            break;

        default:
            break;
        }
    }
}