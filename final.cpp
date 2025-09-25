#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <limits>

class Date {
private:
    int day, month, year;

public:
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = 1 + ltm->tm_mon;
        year = 1900 + ltm->tm_year;
    }
    
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    std::string toString() const {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
    }
    
    std::string toFileString() const {
        return std::to_string(day) + "," + std::to_string(month) + "," + std::to_string(year);
    }
    
    static Date fromFileString(const std::string& str) {
        std::stringstream ss(str);
        std::string item;
        std::vector<int> parts;
        
        while (getline(ss, item, ',')) {
            parts.push_back(stoi(item));
        }
        
        return Date(parts[0], parts[1], parts[2]);
    }
    
    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    
    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }
    
    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }
};

class Expense {
private:
    static int nextId;
    int id;
    double amount;
    std::string category;
    std::string description;
    Date date;

public:
    Expense(double amt, const std::string& cat, const std::string& desc, const Date& d = Date())
        : id(nextId++), amount(amt), category(cat), description(desc), date(d) {}
    
    Expense(int i, double amt, const std::string& cat, const std::string& desc, const Date& d)
        : id(i), amount(amt), category(cat), description(desc), date(d) {
        if (i >= nextId) nextId = i + 1;
    }
    
    int getId() const { return id; }
    double getAmount() const { return amount; }
    std::string getCategory() const { return category; }
    std::string getDescription() const { return description; }
    Date getDate() const { return date; }
    
    void setAmount(double amt) { amount = amt; }
    void setCategory(const std::string& cat) { category = cat; }
    void setDescription(const std::string& desc) { description = desc; }
    
    std::string toString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "ID: " << id << " | Amount: " << amount 
           << " | Category: " << category << " | Description: " << description 
           << " | Date: " << date.toString();
        return ss.str();
    }
    
    std::string toFileString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << id << "," << amount << "," << category << "," << description << "," << date.toFileString();
        return ss.str();
    }
    
    static Expense fromFileString(const std::string& line) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> parts;
        
        while (getline(ss, item, ',')) {
            parts.push_back(item);
        }
        
        if (parts.size() >= 7) {
            int id = stoi(parts[0]);
            double amount = stod(parts[1]);
            std::string category = parts[2];
            std::string description = parts[3];
            Date date(stoi(parts[4]), stoi(parts[5]), stoi(parts[6]));
            
            return Expense(id, amount, category, description, date);
        }
        
        return Expense(0, 0.0, "", "", Date());
    }
};

int Expense::nextId = 1;

// class Budget {
// private:
//     std::map<std::string, double> categoryLimits;
//     double totalBudget;

// public:
//     Budget() : totalBudget(0.0) {}
    
//     void setCategoryBudget(const std::string& category, double limit) {
//         categoryLimits[category] = limit;
//         calculateTotalBudget();
//     }
    
//     void removeCategoryBudget(const std::string& category) {
//         categoryLimits.erase(category);
//         calculateTotalBudget();
//     }
    
//     double getCategoryBudget(const std::string& category) const {
//         auto it = categoryLimits.find(category);
//         return (it != categoryLimits.end()) ? it->second : 0.0;
//     }
    
//     double getTotalBudget() const { return totalBudget; }
    
//     std::map<std::string, double> getAllBudgets() const { return categoryLimits; }
    
//     void calculateTotalBudget() {
//         totalBudget = 0.0;
//         for (const auto& pair : categoryLimits) {
//             totalBudget += pair.second;
//         }
//     }
    
//     std::string toFileString() const {
//         std::stringstream ss;
//         for (const auto& pair : categoryLimits) {
//             ss << pair.first << ":" << std::fixed << std::setprecision(2) << pair.second << ";";
//         }
//         return ss.str();
//     }
    
//     void fromFileString(const std::string& str) {
//         categoryLimits.clear();
//         if (str.empty()) return;
        
//         std::stringstream ss(str);
//         std::string item;
        
//         while (getline(ss, item, ';')) {
//             if (item.empty()) continue;
            
//             size_t colonPos = item.find(':');
//             if (colonPos != std::string::npos) {
//                 std::string category = item.substr(0, colonPos);
//                 double amount = stod(item.substr(colonPos + 1));
//                 categoryLimits[category] = amount;
//             }
//         }
//         calculateTotalBudget();
//     }
// };

class ExpenseTracker {
private:
    std::vector<Expense> expenses;
    //Budget budget;
    std::string csvFile;  // Primary CSV file
    std::string budgetFile;

public:
    ExpenseTracker(const std::string& csvFileName = "expenses.csv") 
                  //const std::string& budgetFileName = "budget.txt")
        : csvFile(csvFileName){
        loadFromCSV();  // Always load from CSV
        //loadBudget();
    }
    
    ~ExpenseTracker() {
        saveToCSV();  // Always save to CSV
        //saveBudget();
    }
    
    void addExpense(double amount, const std::string& category, 
                   const std::string& description, const Date& date = Date()) {
        expenses.emplace_back(amount, category, description, date);
        std::cout << "Expense added successfully!\n";
        
        // Immediately save to CSV
        saveToCSV();
        
        // Check budget warning
        //checkBudgetWarning(category);
    }
    
    void removeExpenseById(int id) {
        auto it = std::find_if(expenses.begin(), expenses.end(),
            [id](const Expense& e) { return e.getId() == id; });
        
        if (it != expenses.end()) {
            expenses.erase(it);
            std::cout << "Expense removed successfully!\n";
            
            // Immediately save to CSV
            saveToCSV();
        } else {
            std::cout << "Expense with ID " << id << " not found!\n";
        }
    }
    
    void editExpenseById(int id) {
        auto it = std::find_if(expenses.begin(), expenses.end(),
            [id](const Expense& e) { return e.getId() == id; });
        
        if (it != expenses.end()) {
            editExpenseHelper(*it);
        } else {
            std::cout << "Expense with ID " << id << " not found!\n";
        }
    }
    
    void editExpenseInteractive() {
        if (expenses.empty()) {
            std::cout << "No expenses recorded.\n";
            return;
        }
        
        std::cout << "\n=== FIND EXPENSE TO EDIT ===\n";
        std::cout << "1. Search by keyword and select\n";
        std::cout << "2. Browse recent expenses\n";
        std::cout << "3. Browse by category\n";
        std::cout << "4. Enter expense ID directly\n";
        std::cout << "Choose search method: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                findAndEditByKeyword();
                break;
            case 2:
                findAndEditRecent();
                break;
            case 3:
                findAndEditByCategory();
                break;
            case 4: {
                int id;
                std::cout << "Enter expense ID: ";
                std::cin >> id;
                editExpenseById(id);
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
        }
    }
    
    void removeExpenseInteractive() {
        if (expenses.empty()) {
            std::cout << "No expenses recorded.\n";
            return;
        }
        
        std::cout << "\n=== FIND EXPENSE TO REMOVE ===\n";
        std::cout << "1. Search by keyword and select\n";
        std::cout << "2. Browse recent expenses\n";
        std::cout << "3. Browse by category\n";
        std::cout << "4. Enter expense ID directly\n";
        std::cout << "Choose search method: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                findAndRemoveByKeyword();
                break;
            case 2:
                findAndRemoveRecent();
                break;
            case 3:
                findAndRemoveByCategory();
                break;
            case 4: {
                int id;
                std::cout << "Enter expense ID: ";
                std::cin >> id;
                removeExpenseById(id);
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
        }
    }
    
    void viewAllExpenses() const {
        if (expenses.empty()) {
            std::cout << "No expenses recorded.\n";
            return;
        }
        
        std::cout << "\n=== ALL EXPENSES ===\n";
        for (const auto& expense : expenses) {
            std::cout << expense.toString() << "\n";
        }
        
        double total = getTotalExpenses();
        std::cout << "\nTotal Expenses: " << std::fixed << std::setprecision(2) << total << "\n";
    }
    
    void viewExpensesByCategory() const {
        if (expenses.empty()) {
            std::cout << "No expenses recorded.\n";
            return;
        }
        
        std::map<std::string, double> categoryTotals;
        for (const auto& expense : expenses) {
            categoryTotals[expense.getCategory()] += expense.getAmount();
        }
        
        std::cout << "\n=== EXPENSES BY CATEGORY ===\n";
        for (const auto& pair : categoryTotals) {
            std::cout << pair.first << ": " << std::fixed << std::setprecision(2) 
                     << pair.second << "\n";
        }
    }
    
    void viewMonthlyReport(int month, int year) const {
        std::vector<Expense> monthlyExpenses;
        double total = 0.0;
        
        //std::cout << "\nDebug: Looking for expenses in " << month << "/" << year << "\n";
        std::cout << "Total expenses in database: " << expenses.size() << "\n";
        
        for (const auto& expense : expenses) {
            Date expenseDate = expense.getDate();
            // std::cout << "Debug: Checking expense date: " << expenseDate.toString() 
                    //  << " (Month: " << expenseDate.getMonth() << ", Year: " << expenseDate.getYear() << ")\n";
            
            if (expenseDate.getMonth() == month && expenseDate.getYear() == year) {
                monthlyExpenses.push_back(expense);
                total += expense.getAmount();
                // std::cout << "  -> MATCH: " << expense.getDescription() << " - $" << expense.getAmount() << "\n";
            }
        }
        
        if (monthlyExpenses.empty()) {
            std::cout << "No expenses found for " << month << "/" << year << "\n";
            std::cout << "Available months/years in data:\n";
            
            // Show available dates for debugging
            std::set<std::pair<int, int>> availableDates;
            for (const auto& expense : expenses) {
                availableDates.insert({expense.getDate().getMonth(), expense.getDate().getYear()});
            }
            
            for (const auto& date : availableDates) {
                std::cout << "  " << date.first << "/" << date.second << "\n";
            }
            return;
        }
        
        std::cout << "\n=== MONTHLY REPORT (" << month << "/" << year << ") ===\n";
        for (const auto& expense : monthlyExpenses) {
            std::cout << expense.toString() << "\n";
        }
        
        std::cout << "\nTotal for " << month << "/" << year << ": " 
                 << std::fixed << std::setprecision(2) << total << "\n";
        
        // Category breakdown for the month
        std::map<std::string, double> categoryTotals;
        for (const auto& expense : monthlyExpenses) {
            categoryTotals[expense.getCategory()] += expense.getAmount();
        }
        
        std::cout << "\nCategory Breakdown:\n";
        for (const auto& pair : categoryTotals) {
            std::cout << "  " << pair.first << ": " << std::fixed << std::setprecision(2) 
                     << pair.second << "\n";
        }
    }
    
    // void setBudget(const std::string& category, double limit) {
    //     budget.setCategoryBudget(category, limit);
    //     std::cout << "Budget set for " << category << ": $" << std::fixed 
    //              << std::setprecision(2) << limit << "\n";
    // }
    
    // void viewBudgetStatus() const {
    //     auto budgets = budget.getAllBudgets();
    //     if (budgets.empty()) {
    //         std::cout << "No budgets set.\n";
    //         return;
    //     }
        
    //     std::cout << "\n=== BUDGET STATUS ===\n";
        
    //     std::map<std::string, double> categorySpending;
    //     for (const auto& expense : expenses) {
    //         categorySpending[expense.getCategory()] += expense.getAmount();
    //     }
        
    //     for (const auto& budgetPair : budgets) {
    //         std::string category = budgetPair.first;
    //         double budgetAmount = budgetPair.second;
    //         double spent = categorySpending[category];
    //         double remaining = budgetAmount - spent;
    //         double percentage = (budgetAmount > 0) ? (spent / budgetAmount) * 100 : 0;
            
    //         std::cout << category << ":\n";
    //         std::cout << "  Budget: $" << std::fixed << std::setprecision(2) << budgetAmount << "\n";
    //         std::cout << "  Spent: $" << std::fixed << std::setprecision(2) << spent << "\n";
    //         std::cout << "  Remaining: $" << std::fixed << std::setprecision(2) << remaining << "\n";
    //         std::cout << "  Usage: " << std::fixed << std::setprecision(1) << percentage << "%\n";
            
    //         if (remaining < 0) {
    //             std::cout << "  WARNING: OVER BUDGET by $" << std::fixed << std::setprecision(2) 
    //                      << -remaining << "\n";
    //         } else if (percentage > 80) {
    //             std::cout << "  WARNING: Over 80% of budget used\n";
    //         }
    //         std::cout << "\n";
    //     }
        
    //     double totalBudget = budget.getTotalBudget();
    //     double totalSpent = getTotalExpenses();
    //     std::cout << "Overall Budget: $" << std::fixed << std::setprecision(2) << totalBudget << "\n";
    //     std::cout << "Total Spent: $" << std::fixed << std::setprecision(2) << totalSpent << "\n";
    //     std::cout << "Remaining: $" << std::fixed << std::setprecision(2) << (totalBudget - totalSpent) << "\n";
    // }
    
    void searchExpenses(const std::string& keyword) const {
        std::vector<Expense> results;
        
        for (const auto& expense : expenses) {
            if (expense.getCategory().find(keyword) != std::string::npos ||
                expense.getDescription().find(keyword) != std::string::npos) {
                results.push_back(expense);
            }
        }
        
        if (results.empty()) {
            std::cout << "No expenses found containing '" << keyword << "'\n";
            return;
        }
        
        std::cout << "\n=== SEARCH RESULTS for '" << keyword << "' ===\n";
        for (const auto& expense : results) {
            std::cout << expense.toString() << "\n";
        }
    }
    
    void getTopCategories(int limit = 5) const {
        std::map<std::string, double> categoryTotals;
        for (const auto& expense : expenses) {
            categoryTotals[expense.getCategory()] += expense.getAmount();
        }
        
        std::vector<std::pair<std::string, double>> sortedCategories(
            categoryTotals.begin(), categoryTotals.end());
        
        std::sort(sortedCategories.begin(), sortedCategories.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        
        std::cout << "\n=== TOP " << limit << " SPENDING CATEGORIES ===\n";
        int count = 0;
        for (const auto& pair : sortedCategories) {
            if (count >= limit) break;
            std::cout << ++count << ". " << pair.first << ": " 
                     << std::fixed << std::setprecision(2) << pair.second << "\n";
        }
    }
    
    void importFromCSV(const std::string& filename) {
        // If importing from the main CSV file, just reload
        if (filename == csvFile) {
            expenses.clear();
            loadFromCSV();
            std::cout << "Reloaded " << expenses.size() << " expenses from " << filename << "\n";
            return;
        }
        
        // Otherwise, merge data from another CSV file
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Error: Could not open file '" << filename << "'\n";
            return;
        }
        
        std::string line;
        int importedCount = 0;
        int lineNumber = 0;
        
        // Skip header line
        if (getline(file, line)) {
            lineNumber++;
            std::cout << "Skipping header: " << line << "\n";
        }
        
        while (getline(file, line)) {
            lineNumber++;
            
            if (line.empty()) continue;
            
            try {
                Expense expense = parseCSVLine(line);
                if (expense.getAmount() > 0) {
                    expenses.push_back(expense);
                    importedCount++;
                }
            } catch (const std::exception& e) {
                std::cout << "Error parsing line " << lineNumber << ": " << line << "\n";
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        
        file.close();
        std::cout << "Successfully imported " << importedCount << " additional expenses from '" << filename << "'\n";
        
        // Save the merged data to main CSV
        if (importedCount > 0) {
            saveToCSV();
            std::cout << "Updated main CSV file with " << expenses.size() << " total expenses\n";
        }
    }
    
    // void exportToCSV(const std::string& filename) const {
    //     if (filename == csvFile) {
    //         std::cout << "This is already your main CSV file. No need to export.\n";
    //         return;
    //     }
        
    //     std::ofstream file(filename);
    //     if (!file.is_open()) {
    //         std::cout << "Error: Could not create file '" << filename << "'\n";
    //         return;
    //     }
        
    //     // Write header
    //     file << "Date,Category,Description,Amount\n";
        
    //     // Write expenses
    //     for (const auto& expense : expenses) {
    //         file << formatDateForCSV(expense.getDate()) << ","
    //              << expense.getCategory() << ","
    //              << expense.getDescription() << ","
    //              << std::fixed << std::setprecision(2) << expense.getAmount() << "\n";
    //     }
        
    //     file.close();
    //     std::cout << "Successfully exported " << expenses.size() 
    //              << " expenses to '" << filename << "'\n";
    // }

private:
    double getTotalExpenses() const {
        double total = 0.0;
        for (const auto& expense : expenses) {
            total += expense.getAmount();
        }
        return total;
    }
    
    // void checkBudgetWarning(const std::string& category) {
    //     double categoryBudget = budget.getCategoryBudget(category);
    //     if (categoryBudget == 0.0) return;
        
    //     double categorySpent = 0.0;
    //     for (const auto& expense : expenses) {
    //         if (expense.getCategory() == category) {
    //             categorySpent += expense.getAmount();
    //         }
    //     }
        
    //     double percentage = (categorySpent / categoryBudget) * 100;
        
    //     if (categorySpent > categoryBudget) {
    //         std::cout << "Budget Alert: You've exceeded your " << category 
    //                  << " budget by $" << std::fixed << std::setprecision(2) 
    //                  << (categorySpent - categoryBudget) << "\n";
    //     } else if (percentage > 80) {
    //         std::cout << "Budget Warning: You've used " << std::fixed 
    //                  << std::setprecision(1) << percentage << "% of your " 
    //                  << category << " budget\n";
    //     }
    // }
    
    void editExpenseHelper(Expense& expense) {
        std::cout << "\nCurrent expense: " << expense.toString() << "\n\n";
        
        double amount;
        std::string category, description;
        
        std::cout << "Enter new amount (current: " << std::fixed << std::setprecision(2) 
                 << expense.getAmount() << "): ";
        std::cin >> amount;
        
        std::cout << "Enter new category (current: " << expense.getCategory() << "): ";
        std::cin.ignore();
        getline(std::cin, category);
        
        std::cout << "Enter new description (current: " << expense.getDescription() << "): ";
        getline(std::cin, description);
        
        expense.setAmount(amount);
        expense.setCategory(category);
        expense.setDescription(description);
        
        std::cout << "Expense updated successfully!\n";
        
        // Immediately save to CSV after editing
        saveToCSV();
    }
    
    void findAndEditByKeyword() {
        std::string keyword;
        std::cout << "Enter search keyword (category or description): ";
        std::cin.ignore();
        getline(std::cin, keyword);
        
        std::vector<const Expense*> matches;
        for (const auto& expense : expenses) {
            if (expense.getCategory().find(keyword) != std::string::npos ||
                expense.getDescription().find(keyword) != std::string::npos) {
                matches.push_back(&expense);
            }
        }
        
        if (matches.empty()) {
            std::cout << "No expenses found containing '" << keyword << "'\n";
            return;
        }
        
        selectAndEditFromList(matches);
    }
    
    void findAndEditRecent() {
        std::vector<const Expense*> sortedExpenses;
        for (const auto& expense : expenses) {
            sortedExpenses.push_back(&expense);
        }
        
        std::sort(sortedExpenses.begin(), sortedExpenses.end(),
            [](const Expense* a, const Expense* b) { return a->getId() > b->getId(); });
        
        int limit = std::min(10, (int)sortedExpenses.size());
        std::vector<const Expense*> recentExpenses(sortedExpenses.begin(), 
                                                   sortedExpenses.begin() + limit);
        
        std::cout << "\n=== RECENT EXPENSES ===\n";
        selectAndEditFromList(recentExpenses);
    }
    
    void findAndEditByCategory() {
        std::set<std::string> categories;
        for (const auto& expense : expenses) {
            categories.insert(expense.getCategory());
        }
        
        if (categories.empty()) {
            std::cout << "No categories found.\n";
            return;
        }
        
        std::cout << "\n=== SELECT CATEGORY ===\n";
        std::vector<std::string> categoryList(categories.begin(), categories.end());
        
        for (size_t i = 0; i < categoryList.size(); ++i) {
            std::cout << (i + 1) << ". " << categoryList[i] << "\n";
        }
        
        std::cout << "Choose category: ";
        int choice;
        std::cin >> choice;
        
        if (choice < 1 || choice > (int)categoryList.size()) {
            std::cout << "Invalid choice.\n";
            return;
        }
        
        std::string selectedCategory = categoryList[choice - 1];
        
        std::vector<const Expense*> categoryExpenses;
        for (const auto& expense : expenses) {
            if (expense.getCategory() == selectedCategory) {
                categoryExpenses.push_back(&expense);
            }
        }
        
        selectAndEditFromList(categoryExpenses);
    }
    
    void selectAndEditFromList(const std::vector<const Expense*>& expenseList) {
        if (expenseList.empty()) {
            std::cout << "No expenses to display.\n";
            return;
        }
        
        std::cout << "\n=== SELECT EXPENSE TO EDIT ===\n";
        for (size_t i = 0; i < expenseList.size(); ++i) {
            std::cout << (i + 1) << ". " << expenseList[i]->toString() << "\n";
        }
        
        std::cout << "\nChoose expense to edit (1-" << expenseList.size() << "): ";
        int choice;
        std::cin >> choice;
        
        if (choice < 1 || choice > (int)expenseList.size()) {
            std::cout << "Invalid choice.\n";
            return;
        }
        
        int selectedId = expenseList[choice - 1]->getId();
        auto it = std::find_if(expenses.begin(), expenses.end(),
            [selectedId](const Expense& e) { return e.getId() == selectedId; });
        
        if (it != expenses.end()) {
            editExpenseHelper(*it);
        }
    }
    
    void findAndRemoveByKeyword() {
        std::string keyword;
        std::cout << "Enter search keyword (category or description): ";
        std::cin.ignore();
        getline(std::cin, keyword);
        
        std::vector<const Expense*> matches;
        for (const auto& expense : expenses) {
            if (expense.getCategory().find(keyword) != std::string::npos ||
                expense.getDescription().find(keyword) != std::string::npos) {
                matches.push_back(&expense);
            }
        }
        
        if (matches.empty()) {
            std::cout << "No expenses found containing '" << keyword << "'\n";
            return;
        }
        
        selectAndRemoveFromList(matches);
    }
    
    void findAndRemoveRecent() {
        std::vector<const Expense*> sortedExpenses;
        for (const auto& expense : expenses) {
            sortedExpenses.push_back(&expense);
        }
        
        std::sort(sortedExpenses.begin(), sortedExpenses.end(),
            [](const Expense* a, const Expense* b) { return a->getId() > b->getId(); });
        
        int limit = std::min(10, (int)sortedExpenses.size());
        std::vector<const Expense*> recentExpenses(sortedExpenses.begin(), 
                                                   sortedExpenses.begin() + limit);
        
        std::cout << "\n=== RECENT EXPENSES ===\n";
        selectAndRemoveFromList(recentExpenses);
    }
    
    void findAndRemoveByCategory() {
        std::set<std::string> categories;
        for (const auto& expense : expenses) {
            categories.insert(expense.getCategory());
        }
        
        if (categories.empty()) {
            std::cout << "No categories found.\n";
            return;
        }
        
        std::cout << "\n=== SELECT CATEGORY ===\n";
        std::vector<std::string> categoryList(categories.begin(), categories.end());
        
        for (size_t i = 0; i < categoryList.size(); ++i) {
            std::cout << (i + 1) << ". " << categoryList[i] << "\n";
        }
        
        std::cout << "Choose category: ";
        int choice;
        std::cin >> choice;
        
        if (choice < 1 || choice > (int)categoryList.size()) {
            std::cout << "Invalid choice.\n";
            return;
        }
        
        std::string selectedCategory = categoryList[choice - 1];
        
        std::vector<const Expense*> categoryExpenses;
        for (const auto& expense : expenses) {
            if (expense.getCategory() == selectedCategory) {
                categoryExpenses.push_back(&expense);
            }
        }
        
        selectAndRemoveFromList(categoryExpenses);
    }
    
    void selectAndRemoveFromList(const std::vector<const Expense*>& expenseList) {
        if (expenseList.empty()) {
            std::cout << "No expenses to display.\n";
            return;
        }
        
        std::cout << "\n=== SELECT EXPENSE TO REMOVE ===\n";
        for (size_t i = 0; i < expenseList.size(); ++i) {
            std::cout << (i + 1) << ". " << expenseList[i]->toString() << "\n";
        }
        
        std::cout << "\nChoose expense to remove (1-" << expenseList.size() << "): ";
        int choice;
        std::cin >> choice;
        
        if (choice < 1 || choice > (int)expenseList.size()) {
            std::cout << "Invalid choice.\n";
            return;
        }
        
        int selectedId = expenseList[choice - 1]->getId();
        
        std::cout << "\nAre you sure you want to remove this expense? (y/n): ";
        char confirm;
        std::cin >> confirm;
        
        if (confirm == 'y' || confirm == 'Y') {
            removeExpenseById(selectedId);
        } else {
            std::cout << "Removal cancelled.\n";
        }
    }
    
    Expense parseCSVLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;
        
        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                fields.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
        fields.push_back(field);
        
        if (fields.size() < 4) {
            throw std::runtime_error("Invalid CSV format");
        }
        
        Date date = parseDate(fields[0]);
        std::string category = fields[1];
        std::string description = fields[2];
        double amount = std::stod(fields[3]);
        
        return Expense(amount, category, description, date);
    }
    
    Date parseDate(const std::string& dateStr) {
        std::vector<int> parts;
        std::stringstream ss(dateStr);
        std::string part;
        
        while (getline(ss, part, '-')) {
            parts.push_back(std::stoi(part));
        }
        
        if (parts.size() != 3) {
            throw std::runtime_error("Invalid date format");
        }
        
        return Date(parts[0], parts[1], parts[2]);
    }
    
    void loadFromCSV() {
        expenses.clear();
        
        std::ifstream file(csvFile);
        if (!file.is_open()) {
            std::cout << "CSV file '" << csvFile << "' not found. Starting with empty expense list.\n";
            return;
        }
        
        std::string line;
        int lineNumber = 0;
        
        // Skip header line if it exists
        if (getline(file, line)) {
            lineNumber++;
            // Check if first line is a header
            if (line.find("Date,Category,Description,Amount") != std::string::npos ||
                line.find("date,category,description,amount") != std::string::npos) {
                // It's a header, skip it
                std::cout << "Found CSV header, loading data...\n";
            } else {
                // First line is data, parse it
                try {
                    Expense expense = parseCSVLine(line);
                    if (expense.getAmount() > 0) {
                        expenses.push_back(expense);
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error parsing first line: " << e.what() << "\n";
                }
            }
        }
        
        // Parse remaining lines
        while (getline(file, line)) {
            lineNumber++;
            
            if (line.empty()) continue;
            
            try {
                Expense expense = parseCSVLine(line);
                if (expense.getAmount() > 0) {
                    expenses.push_back(expense);
                }
            } catch (const std::exception& e) {
                std::cout << "Error parsing line " << lineNumber << ": " << e.what() << "\n";
            }
        }
        
        file.close();
        std::cout << "Loaded " << expenses.size() << " expenses from '" << csvFile << "'\n";
    }
    
    void saveToCSV() const {
        std::ofstream file(csvFile);
        if (!file.is_open()) {
            std::cout << "Error: Could not save to '" << csvFile << "'\n";
            return;
        }
        
        // Write header
        file << "Date,Category,Description,Amount\n";
        
        // Write expenses in CSV format (DD-MM-YYYY)
        for (const auto& expense : expenses) {
            file << formatDateForCSV(expense.getDate()) << ","
                 << expense.getCategory() << ","
                 << expense.getDescription() << ","
                 << std::fixed << std::setprecision(2) << expense.getAmount() << "\n";
        }
        
        file.close();
        std::cout << "Saved " << expenses.size() << " expenses to '" << csvFile << "'\n";
    }
    
    std::string formatDateForCSV(const Date& date) const {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << date.getDay() << "-"
           << std::setfill('0') << std::setw(2) << date.getMonth() << "-"
           << date.getYear();
        return ss.str();
    }
    
    // void saveBudget() const {
    //     std::ofstream file(budgetFile);
    //     if (file.is_open()) {
    //         file << budget.toFileString() << "\n";
    //         file.close();
    //     }
    // }
    
    // void loadBudget() {
    //     std::ifstream file(budgetFile);
    //     if (file.is_open()) {
    //         std::string line;
    //         if (getline(file, line)) {
    //             budget.fromFileString(line);
    //         }
    //         file.close();
    //     }
    // }
};

void displayMenu() {
    std::cout << "\n=== PERSONAL EXPENSE TRACKER ===\n";
    std::cout << "1. Add Expense\n";
    std::cout << "2. View All Expenses\n";
    std::cout << "3. View Expenses by Category\n";
    std::cout << "4. Edit Expense (Smart Search)\n";
    std::cout << "5. Remove Expense (Smart Search)\n";
    std::cout << "6. Search Expenses\n";
    std::cout << "7. Monthly Report\n";
    //std::cout << "8. Set Budget\n";
    //std::cout << "9. View Budget Status\n";
    std::cout << "8. Top Spending Categories\n";
    std::cout << "9. Import from Another CSV\n";
    //std::cout << "1. Export to CSV\n";
    //std::cout << "13. Reload CSV Data\n";
    std::cout << "10. Exit\n";
    std::cout << "Choose an option: ";
}

int main() {
    ExpenseTracker tracker;
    int choice;
    
    std::cout << "Welcome to Personal Expense Tracker!\n";
    
    while (true) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        switch (choice) {
            case 1: {
                double amount;
                std::string category, description;
                
                std::cout << "Enter amount: ";
                std::cin >> amount;
                
                std::cout << "Enter category: ";
                std::cin.ignore();
                getline(std::cin, category);
                
                std::cout << "Enter description: ";
                getline(std::cin, description);
                
                tracker.addExpense(amount, category, description);
                break;
            }
            
            case 2:
                tracker.viewAllExpenses();
                break;
                
            case 3:
                tracker.viewExpensesByCategory();
                break;
                
            case 4:
                tracker.editExpenseInteractive();
                break;
                
            case 5:
                tracker.removeExpenseInteractive();
                break;
            
            case 6: {
                std::string keyword;
                std::cout << "Enter search keyword: ";
                std::cin.ignore();
                getline(std::cin, keyword);
                tracker.searchExpenses(keyword);
                break;
            }
            
            case 7: {
                int month, year;
                std::cout << "Enter month (1-12): ";
                std::cin >> month;
                std::cout << "Enter year: ";
                std::cin >> year;
                tracker.viewMonthlyReport(month, year);
                break;
            }
            
            // case 8: {
            //     std::string category;
            //     double limit;
            //     std::cout << "Enter category: ";
            //     std::cin.ignore();
            //     getline(std::cin, category);
            //     std::cout << "Enter budget limit: $";
            //     std::cin >> limit;
            //     //tracker.setBudget(category, limit);
            //     break;
            // }
            
            // case 9:
            //     tracker.viewBudgetStatus();
            //     break;
                
            case 8: {
                int limit;
                std::cout << "How many top categories to show (default 5): ";
                std::cin >> limit;
                tracker.getTopCategories(limit > 0 ? limit : 5);
                break;
            }
            
            case 9: {
                std::string filename;
                std::cout << "Enter CSV filename to import (e.g., expenses.csv): ";
                std::cin.ignore();
                getline(std::cin, filename);
                tracker.importFromCSV(filename);
                break;
            }
            
            // case 12: {
            //     std::string filename;
            //     std::cout << "Enter CSV filename to export (e.g., my_expenses.csv): ";
            //     std::cin.ignore();
            //     getline(std::cin, filename);
            //     tracker.exportToCSV(filename);
            //     break;
            // }
            
            // case 13: {
            //     std::cout << "Reloading data from expenses.csv...\n";
            //     tracker.importFromCSV("expenses.csv");
            //     break;
            // }
            
            case 10:
                std::cout << "Thank you for using Personal Expense Tracker!\n";
                return 0;
                
            default:
                std::cout << "Invalid choice. Please try again.\n";
                break;
        }
    }
    
    return 0;
}