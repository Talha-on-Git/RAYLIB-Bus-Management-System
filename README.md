# Student Point Management System

## Introduction
Managing student bus commutes manually is inefficient and prone to errors. Static timetables and non-visual applications make it difficult to track seat availability, routes, and timings. This system addresses these problems by offering an intuitive graphical interface where students can:  

- View all bus routes visually.  
- Check and book available seats in real-time.  
- Access clear schedules for morning and evening trips.  

By integrating these features into a single platform, the project aims to make commuting simpler, faster, and more reliable.

## System Design

### Overview
The program flows as follows:  
Start → Login → Main Dashboard → (Route Map | Seat Availability | Book Seat | Time Schedule) → Exit


### Key Features
- **Interactive Route Map:** Students can visually track bus routes and stops on a 2D map.  
- **Seat Management:** Check available seats and book them directly through the GUI.  
- **Schedule Panel:** View departure and arrival times for morning and evening trips.  
- **User-Friendly Menus:** Smooth navigation between all modules.  
- **File Handling:** Stores schedules and seat bookings locally in `.txt` files.

---

## Implementation
- **Programming Language:** C  
- **Library/Framework:** Raylib for GUI and 2D rendering  
- **Data Storage:** Local flat files (`schedules.txt`, `seats.txt`, `routes.txt`)  
- **Operating System:** Cross-platform (Windows, Linux)
- 
## Testing & Results
The system was tested on multiple scenarios including:

- Booking seats on different buses.
- Checking seat availability before and after booking.
- Displaying morning and evening schedules for all routes.

**Result:** All modules worked as intended. Seat booking updates the availability correctly, and schedules display accurately. The GUI responded efficiently to user inputs with minimal delays.

---

## Limitations

- Only works on systems with Raylib installed.
- Seat and schedule data stored in flat text files; no database integration.
- Graphics are 2D and simple; advanced animations are not included.
- Single-user system; does not support multiple accounts simultaneously.

---

## Future Enhancements

- Integrate a database (e.g., MySQL or MongoDB) for persistent storage.
- Multi-user login system to manage different student accounts.
- Add notifications for bus arrival times and seat confirmations.
- Improve GUI with enhanced graphics, animations, and dynamic route maps.

---

## File Structure

- `raylib.c` – Main source code of the application.
- `schedules.txt` – Stores bus schedules for morning and evening trips.
- `seats.txt` – Stores seat availability and booking records.
- `routes.txt` – Stores bus route information.

---

## Author

**Talha Yousuf**,
**Aryaan Taimur**
