/*
 * GroceryTracker.cpp
 * CS 210 - Project Three: Corner Grocer Item Tracker
 *
 * Description:
 *   Reads a daily purchase log (CS210_Project_Three_Input_File.txt), tallies
 *   the frequency of every item, writes a backup file (frequency.dat), and
 *   presents an interactive menu with four options:
 *     1 - Look up the frequency of a single item
 *     2 - Print all items with their frequencies
 *     3 - Print a text histogram of all item frequencies
 *     4 - Exit the program
 *
 * Author : [Your Name]
 * Date   : 2026-04-25
 */

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <limits>
#include <algorithm>

 // ─────────────────────────────────────────────────────────────────────────────
 // Class: GroceryTracker
 //   Encapsulates all item-frequency logic and file I/O for the Corner Grocer.
 // ─────────────────────────────────────────────────────────────────────────────
class GroceryTracker {
public:
    // ── Constructor ─────────────────────────────────────────────────────────
    // Reads the input file and builds the frequency map, then writes the
    // backup data file.  Both steps happen automatically at startup.
    explicit GroceryTracker(const std::string& inputFilename,
        const std::string& backupFilename)
        : m_inputFilename(inputFilename),
        m_backupFilename(backupFilename)
    {
        LoadFrequencies();   // populate m_frequencyMap from the input file
        WriteBackupFile();   // create / overwrite frequency.dat
    }

    // ── Option 1: Look up a single item ─────────────────────────────────────
    // Returns the frequency of 'item' (case-insensitive comparison).
    // Returns 0 if the item was never purchased.
    int GetFrequency(const std::string& item) const {
        // Normalise input to Title Case to match stored keys
        std::string key = NormaliseKey(item);
        auto it = m_frequencyMap.find(key);
        if (it != m_frequencyMap.end()) {
            return it->second;
        }
        return 0;  // item not found
    }

    // ── Option 2: Print all items with numeric frequencies ──────────────────
    void PrintFrequencies() const {
        std::cout << "\n--- Item Purchase Frequencies ---\n";
        for (const auto& entry : m_frequencyMap) {
            // Left-align item name in a 15-char field, then print count
            std::cout << std::left;
            std::cout.width(15);
            std::cout << entry.first << entry.second << "\n";
        }
        std::cout << "---------------------------------\n";
    }

    // ── Option 3: Print histogram (one '*' per purchase) ────────────────────
    void PrintHistogram() const {
        std::cout << "\n--- Item Purchase Histogram ---\n";
        for (const auto& entry : m_frequencyMap) {
            // Print item name padded to 15 chars
            std::cout << std::left;
            std::cout.width(15);
            std::cout << entry.first;

            // Print one asterisk for every purchase
            std::cout << std::string(entry.second, '*') << "\n";
        }
        std::cout << "-------------------------------\n";
    }

private:
    // ── Private member data ──────────────────────────────────────────────────
    std::string              m_inputFilename;   // path to the input purchase log
    std::string              m_backupFilename;  // path for the backup data file
    std::map<std::string, int> m_frequencyMap; // item name → purchase count

    // ── LoadFrequencies ──────────────────────────────────────────────────────
    // Opens the input file, reads one item per line, and increments its count
    // in m_frequencyMap.  Exits with an error message if the file cannot be
    // opened.
    void LoadFrequencies() {
        std::ifstream inFile(m_inputFilename);
        if (!inFile.is_open()) {
            std::cerr << "ERROR: Could not open input file \""
                << m_inputFilename << "\".\n"
                << "Make sure the file is in the same directory as the "
                "executable.\n";
            std::exit(EXIT_FAILURE);
        }

        std::string item;
        while (std::getline(inFile, item)) {
            // Trim trailing whitespace / carriage returns (Windows line endings)
            while (!item.empty() &&
                (item.back() == '\r' || item.back() == ' ' ||
                    item.back() == '\t')) {
                item.pop_back();
            }
            if (item.empty()) continue;  // skip blank lines

            // Normalise capitalisation before storing
            std::string key = NormaliseKey(item);
            m_frequencyMap[key]++;  // default-initialises to 0 on first access
        }
        inFile.close();
    }

    // ── WriteBackupFile ──────────────────────────────────────────────────────
    // Writes every item and its frequency to the backup data file in the format
    //   ItemName Count
    // One entry per line, matching the format required by the spec.
    void WriteBackupFile() const {
        std::ofstream outFile(m_backupFilename);
        if (!outFile.is_open()) {
            std::cerr << "WARNING: Could not write backup file \""
                << m_backupFilename << "\".\n";
            return;  // non-fatal: the program can still run without the backup
        }

        for (const auto& entry : m_frequencyMap) {
            outFile << entry.first << " " << entry.second << "\n";
        }
        outFile.close();
    }

    // ── NormaliseKey ─────────────────────────────────────────────────────────
    // Converts a string to Title Case (first letter upper, rest lower) so that
    // look-ups are case-insensitive.
    static std::string NormaliseKey(const std::string& raw) {
        if (raw.empty()) return raw;
        std::string key = raw;
        // Convert entire string to lower case first
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        // Capitalise the first character
        key[0] = static_cast<char>(::toupper(key[0]));
        return key;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// PrintMenu
//   Displays the four-option menu to the console.
// ─────────────────────────────────────────────────────────────────────────────
void PrintMenu() {
    std::cout << "\n========== Corner Grocer Menu ==========\n"
        << "  1. Search for an item's frequency\n"
        << "  2. Display all item frequencies\n"
        << "  3. Display frequency histogram\n"
        << "  4. Exit\n"
        << "========================================\n"
        << "Enter your choice (1-4): ";
}

// ─────────────────────────────────────────────────────────────────────────────
// GetValidMenuChoice
//   Reads an integer from stdin, reprompting on invalid input.
//   Returns a value in [1, 4].
// ─────────────────────────────────────────────────────────────────────────────
int GetValidMenuChoice() {
    int choice = 0;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 4) {
            // Clear the error state and discard bad input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 1 and 4: ";
        }
        else {
            // Discard the rest of the line (e.g. newline character)
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────
int main() {
    // File names as constants — easy to change if needed
    const std::string INPUT_FILE = "CS210_Project_Three_Input_File.txt";
    const std::string BACKUP_FILE = "frequency.dat";

    // Instantiate the tracker; constructor loads data and writes backup file
    GroceryTracker tracker(INPUT_FILE, BACKUP_FILE);
    std::cout << "Data loaded successfully. Backup written to \""
        << BACKUP_FILE << "\".\n";

    bool running = true;
    while (running) {
        PrintMenu();
        int choice = GetValidMenuChoice();

        switch (choice) {
            // ── Option 1: Single-item frequency look-up ───────────────────
        case 1: {
            std::cout << "Enter the item name to search: ";
            std::string searchItem;
            std::getline(std::cin, searchItem);

            int freq = tracker.GetFrequency(searchItem);
            if (freq > 0) {
                std::cout << "\"" << searchItem << "\" was purchased "
                    << freq << " time(s) today.\n";
            }
            else {
                std::cout << "\"" << searchItem
                    << "\" was not found in today's records.\n";
            }
            break;
        }

              // ── Option 2: Full frequency list ─────────────────────────────
        case 2:
            tracker.PrintFrequencies();
            break;

            // ── Option 3: Histogram ───────────────────────────────────────
        case 3:
            tracker.PrintHistogram();
            break;

            // ── Option 4: Exit ────────────────────────────────────────────
        case 4:
            std::cout << "Thank you for using the Corner Grocer Tracker. "
                "Goodbye!\n";
            running = false;
            break;

            // Should never reach here due to GetValidMenuChoice validation
        default:
            std::cout << "Unexpected error. Please try again.\n";
            break;
        }
    }

    return EXIT_SUCCESS;
}
