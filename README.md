# Corner Grocer Item Tracker
Author: Nida Qadri

About This Repository
The Corner Grocer needed a way to analyze its daily purchase records to better understand customer buying habits and reorganize its produce section for maximum efficiency. The input data was a plain-text log of every item purchased throughout the day, listed in chronological order.

This program solves that problem by reading the log, counting how many times each item was purchased, and presenting that information through a simple four-option console menu:
Option 1 – Look up the purchase frequency of a specific item
Option 2 – Display all items alongside their numeric purchase counts
Option 3 – Display a text-based histogram (one `*` per purchase) for every item
Option 4 – Exit the program

At startup the program also automatically generates a backup file (`frequency.dat`) containing the full frequency data, requiring no action from the user.

What did I do particularly well?
The overall structure of the program is something I am proud of. Wrapping all data-loading, file I/O, and output logic inside a single `GroceryTracker` class with clearly separated public and private sections keeps the `main` function clean and readable — it only handles the menu loop and user interaction. I also paid close attention to robustness: the input parser trims Windows-style carriage returns (`\r`) so the program works correctly regardless of which operating system created the text file, and the menu validator loops until the user enters a valid integer between 1 and 4, preventing crashes from bad input.

Where could I enhance the code?
Several improvements would make the program more capable and production-ready:
Persistent history – currently the program only processes one file per run. A future version could append multiple daily logs and track trends over time.
Sorted output options – results are currently displayed in alphabetical order because `std::map` sorts by key. Adding an option to sort by frequency (highest to lowest) would make it easier to identify the most popular items at a glance.
Unit tests – adding a small test suite (using a framework like Google Test) would catch regressions if the code is modified later.
Configuration file – hardcoding the input and backup filenames as string constants works for this project, but reading them from a config file or command-line arguments would make the program more flexible and reusable.

What was most challenging, and how did I overcome it?
The trickiest part was diagnosing why the program could not find the input file when running inside Visual Studio. The executable lives in `x64\Debug\`, but Visual Studio sets the working directory to the project source folder by default, so the file path the program searched was different from where the file actually existed. I worked through the problem systematically — checking the error message, tracing the executable path shown in the console output, and testing different folder locations — until the correct directory was identified. This experience reinforced the importance of understanding how an IDE manages working directories separately from build output directories. Going forward I will add this check to my standard debugging workflow and consult Microsoft's documentation on project property pages when environment issues arise.

What skills from this project are transferable?
Several concepts from this project apply directly to future coursework and professional work:
`std::map` and associative containers – the pattern of mapping a key to an accumulated value (frequency counting) appears constantly in data processing, caching, and analytics problems.
Class design with public/private separation – encapsulating data and behavior behind a clean interface is a foundational object-oriented principle used in every language.
File I/O – reading from and writing to files is a universal skill needed in virtually every real-world application.
Input validation – defending against unexpected user input is an industry standard practice that applies to web forms, APIs, command-line tools, and beyond.

How did I make the program maintainable, readable, and adaptable?
Maintainability was a priority throughout. Every function has an inline comment block describing what it does, what it expects, and what it returns. Variable and function names are descriptive and follow a consistent convention (`m_` prefix for private member variables, PascalCase for functions). The two filenames are defined as named constants at the top of `main` rather than scattered as magic strings, so changing them requires editing exactly one place. The `NormaliseKey` helper function centralises the case-conversion logic, meaning if the normalisation strategy ever changes it only needs to be updated in one location. Together these decisions make the codebase easy to read for someone encountering it for the first time and easy to extend without unintended side effects.
