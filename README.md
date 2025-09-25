# Personal-Expense-Tracker
This is simple C++ based  command-line application to manage daily expenses. It lets users add, edit, delete, and search expenses by category or date, with data stored in CSV files. The project applies C++ OOP, file handling, and STL for real-world expense management.


## Features

- **Add Expenses**: Record expenses with amount, category, description, and automatic date stamping
- **View All Expenses**: Display complete expense history with totals
- **Category Analysis**: View expenses grouped by categories
- **Smart Search & Edit**: Multiple ways to find and edit expenses:
  - Search by keywords
  - Browse recent expenses
  - Filter by category
  - Direct ID lookup
- **Smart Search & Remove**: Same intelligent search options for removing expenses
- **Advanced Search**: Find expenses by keyword in category or description
- **Monthly Reports**: Generate detailed reports for specific months/years
- **Top Categories**: Analyze spending patterns by viewing top spending categories
- **CSV Import**: Import expense data from external CSV files
- **Automatic Data Persistence**: All data automatically saved to CSV format

## Technical Specifications

- **Language**: C++11 or higher
- **Storage**: CSV file format (`expenses.csv`)
- **Date Format**: DD-MM-YYYY for CSV storage, DD/MM/YYYY for display
- **Dependencies**: Standard C++ libraries only
- **Platform**: Cross-platform (Windows, Linux, macOS)

## Installation & Setup

### Prerequisites
- C++ compiler (GCC, Clang, or MSVC)
- Make (optional)

### Compilation

#### Using g++
```bash
g++ -std=c++11 -o expense_tracker main.cpp
```

#### Using clang++
```bash
clang++ -std=c++11 -o expense_tracker main.cpp
```

#### Using Visual Studio (Windows)
```bash
cl /EHsc main.cpp
```

## Usage

### Running the Application
```bash
./expense_tracker
```

### Menu Options

1. **Add Expense** - Record new expenses
2. **View All Expenses** - Display all recorded expenses
3. **View Expenses by Category** - Category-wise expense summary
4. **Edit Expense (Smart Search)** - Find and modify expenses
5. **Remove Expense (Smart Search)** - Find and delete expenses
6. **Search Expenses** - Search by keywords
7. **Monthly Report** - Generate month/year specific reports
8. **Top Spending Categories** - View highest spending categories
9. **Import from CSV** - Import data from external CSV files
10. **Exit** - Save and close application

### CSV File Format

The application uses the following CSV format:
```csv
Date,Category,Description,Amount
26-09-2025,Food,Lunch at restaurant,25.50
25-09-2025,Transportation,Bus ticket,3.00
```

### Example Usage

#### Adding an Expense
```
Enter amount: 45.99
Enter category: Groceries
Enter description: Weekly grocery shopping
```

#### Monthly Report
```
Enter month (1-12): 9
Enter year: 2025
```


## Classes Overview

### Date Class
- Handles date operations and formatting
- Automatic current date generation
- CSV-compatible date formatting

### Expense Class
- Represents individual expense records
- Unique ID generation
- String formatting for display and storage

### ExpenseTracker Class
- Main application logic
- CSV file operations
- Search and filter functionality
- Data management

## Key Features Explained

### Smart Search System
The application provides multiple search methods:
- **Keyword Search**: Find expenses containing specific terms
- **Recent Browse**: Show last 10 expenses
- **Category Filter**: List expenses by category
- **Direct ID**: Access expense by unique identifier

### Data Persistence
- Automatic CSV saving after every modification
- Robust CSV parsing with error handling
- Header detection and proper formatting

### Monthly Reporting
- Filter expenses by month and year
- Category breakdown for each month
- Available date ranges display

## Development

### Code Structure
- **Object-Oriented Design**: Clean separation of concerns
- **RAII Pattern**: Automatic resource management
- **Exception Handling**: Robust error handling for file operations
- **STL Usage**: Leverages standard library containers and algorithms

### Future Enhancement Ideas
- Budget tracking system
- Expense categories management
- Data export to different formats
- Graphical user interface
- Multi-currency support
- Receipt attachment system

## Troubleshooting

### Common Issues

**CSV File Not Found**
- The application creates `expenses.csv` automatically on first run
- Ensure write permissions in the application directory

**Import Errors**
- Check CSV format matches: `Date,Category,Description,Amount`
- Ensure date format is DD-MM-YYYY
- Verify no special characters in data

**Compilation Errors**
- Ensure C++11 or higher standard
- Check all required headers are available


