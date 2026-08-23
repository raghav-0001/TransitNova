
# 🚍 TransitNova

## Real-Time Bus Transit Operations, Live Tracking & Journey Planning System

**TransitNova** is a bus transit simulation and management system developed using **C++20, Data Structures & Algorithms, and web technologies**.

It simulates **22 buses** operating between **Ratnapark Buspark and Bode Buspark** across **18 stops**, with buses travelling in both directions. The C++ backend manages the complete transit system while the frontend provides live visualization and user interaction.

---

## ✨ Features

- 🚌 Live bus tracking and route visualization
- 🧭 Journey planner with estimated travel times
- 🔎 Bus search by bus number
- 🅿️ FIFO terminal bus queues
- 🔧 Bus maintenance and restoration
- 🔄 Continuous bidirectional bus simulation
- ⏱️ Controlled bus dispatching
- 🌐 C++ HTTP/JSON API
- 🌗 Dark and light frontend themes

---

## 🛣️ Route

```text
Ratnapark Buspark
→ Singha Durbar
→ Maitighar
→ Bijulibazar
→ Baneshwor
→ Civil
→ Shantinagar
→ Tinkune
→ Koteshwor
→ Jadibuti
→ Lokanthali
→ Kaushaltar
→ Gatthaghar
→ Thimi
→ Bode Chardobato
→ Bode Planning
→ Jaya School
→ Bode Buspark
````

The route operates in both directions:

```text
Ratnapark → Bode
Bode → Ratnapark
```

---

## 🧠 Data Structures & Algorithms

| Data Structure         | Actual Use                                          |
| ---------------------- | --------------------------------------------------- |
| **Linked List**        | Active buses, maintenance buses and terminal queues |
| **Queue**              | FIFO bus dispatching using the Linked List          |
| **Doubly Linked List** | Bidirectional 18-stop route                         |
| **Binary Search Tree** | Bus indexing and searching                          |
| **Arrays**             | Route and travel-time data                          |
| **Vector**             | Temporary collections and journey results           |
| **Threads + Mutex**    | Continuous simulation and synchronization           |
| **Sorting**            | Ranking buses by estimated arrival time             |


---

## 🔄 Bus Simulation

Buses continuously move between route stops using recorded travel times.

```text
Buspark Queue
     ↓
Dispatch
     ↓
Running
     ↓
Opposite Buspark
     ↓
Departure Queue
     ↓
Running Again
```

Each terminal has its own dispatch timer. When a bus reaches a terminal, it is placed into that terminal's queue and can later be dispatched in the opposite direction.

---

## 🧭 Journey Planner

The Journey Planner:

1. Finds the selected stops.
2. Determines the required direction.
3. Calculates travel time using route transition times.
4. Finds suitable active buses.
5. Calculates their estimated arrival.
6. Returns the **top 3 nearest buses**.

---

## 🔧 Maintenance

Buses can be removed from service and placed into a maintenance list.

```text
Running / Parked
       ↓
 Maintenance
       ↓
    Restore
       ↓
Ratnapark Queue
```

Restored buses can re-enter normal operation.

---

## 🌐 Backend API

The backend uses **cpp-httplib** and **nlohmann/json**.

| Endpoint                        | Purpose                 |
| ------------------------------- | ----------------------- |
| `GET /api/status`               | Backend status          |
| `GET /api/buses`                | Current fleet data      |
| `POST /api/journey`             | Journey planning        |
| `POST /api/search`              | Bus search              |
| `POST /api/maintenance`         | Send bus to maintenance |
| `POST /api/maintenance/restore` | Restore bus             |

---

## 🏗️ Architecture

```text
Frontend
HTML / CSS / JavaScript
        │
     HTTP/JSON
        │
        ▼
C++ Backend
        │
   BusManager
        │
 ┌──────┼────────┐
 │      │        │
Lists   BST    Route
 │               │
 └───────┬───────┘
         ↓
  Simulation Engine
```

The simulation runs on a separate thread, while a mutex protects shared backend state.

---

## 📂 Project Structure

```text
TransitNova/
├── backend/
│   ├── include/
│   └── src/
├── frontend/
│   ├── images/
│   ├── index.html
│   ├── journey.html
│   ├── journey.js
│   ├── search.html
│   ├── search.js
│   ├── script.js
│   └── style.css
├── Dockerfile
└── readme.md
```

---

## 🛠️ Technology Stack

**Backend:** C++20, cpp-httplib, nlohmann/json, Threads, Mutex

**Frontend:** HTML5, CSS3, JavaScript, Lucide Icons

**Tools:** GCC/G++, Git, GitHub, VS Code, Docker

---

## 🚀 Running Locally

```bash
git clone https://github.com/raghav-0001/TransitNova.git
cd TransitNova

g++ -std=c++20 backend/src/*.cpp \
    -Ibackend/include \
    -Ibackend \
    -o TransitNova \
    -pthread

./TransitNova
```

Then open:

```text
http://localhost:8080
```

---

## 📌 Project Status

### Version 1.0 — Final

TransitNova includes:

* ✅ 22-bus simulation
* ✅ 18-stop bidirectional route
* ✅ Linked List, Doubly Linked List and BST
* ✅ FIFO terminal queues
* ✅ Live simulation
* ✅ Journey Planner
* ✅ Bus Search
* ✅ Maintenance system
* ✅ HTTP/JSON API
* ✅ Thread synchronization
* ✅ Docker support

---

## 👨‍💻 Team

| Name                | Roll Number |
| ------------------- | ----------- |
| **Raghav Phuyal**   | 081BCT063   |
| **Shrawan Katuwal** | 081BCT077   |
| **Unique Kharel**   | 081BCT092   |

---

<div align="center">

# 🚍 TransitNova

**Smart · Reliable · Efficient**

*Built with C++20, Data Structures & Algorithms, and Web Technologies*

</div>
```
