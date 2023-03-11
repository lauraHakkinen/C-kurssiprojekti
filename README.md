# C-kurssiprojekti
A project made for C programming course. I implemented a calendar where the user can add and remove entries and save them into a file.

The calendar can be used by committing following commands:

- A description month day hour
    - Add entry: Add an entry with the given description to the appointment system. The month, day and hour must be reasonable. (However, you can assume (unrealistically) that all months have the same number of days (up to 31).) 
    - Example: A Haircut 3 26 14 -> book a haircut on 26.3. at 2 p.m.

- D month day hour
    - Delete entry: Deletes the entry from the given time. 
    - Example: D 3 26 14 -> deletes the entry added above.

- L
    - Print the calendar: Prints all its current bookings in the order according to the entry time: the earliest entry first and the latest last.

- W file name
    - Save calendar: Saves all appointments to the file with the given file name.

- O file name
    - Download calendar: Download the calendar from the file, replacing the previous calendar in the memory.

- Q
    - Exit the program: Exits the program and frees all used memory. Detect possible memory leaks.
