#include<iostream>
#include <unordered_map>
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>
#include <algorithm>
using namespace std;


class User {
private:
    string name;
    bool status;    // user status
    int age;
    vector<string> friends;
    vector<pair<string,string>> inbox;     //{sender, message}
public:
    User () {
        name = "";
        age = 0;
        status = false;
    }

    User (string name, int age) {
        this->name = name;
        this->age = age;
        status = false;
    }

    string getName () {
        return name;
    }

    int getAge() {
        return age;
    }

    bool getStatus() {
        return status;
    }

    void setStatus (bool status) {
        this->status = status;
    }

    int partition(vector<string>& arr, int low, int high) {
        string pivot = arr[high];   
        int i = low - 1;

        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) { 
                i++;
                swap(arr[i], arr[j]);
            }
        }

        swap(arr[i + 1], arr[high]);
        return i + 1;
    }


    void quickSort(vector<string>& arr, int low, int high) {
        if (low < high) {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    void sortfriends () {
        quickSort(friends,0,friends.size() - 1);
    }

    bool findFriend (string name) {
        sortfriends ();
        int st = 0, end = friends.size();

        while (st <= end)
        {
            int mid = st + (end - st)/2;
            if (friends[mid] == name) {
                return true;
            }
            else if (friends[mid] < name) {
                st = mid + 1;
            }
            else {
                end = mid - 1;
            }
        }
        return false;
    }

    void addfriend (string friendname) {
        // checking if freind already exist
      if (find(friends.begin(), friends.end(), friendname) == friends.end()) {
            friends.push_back(friendname);
            cout << "Friend " << friendname << " added.\n";
        } 
        else {
            cout << "Already a friend.\n";
        }
    }

    void removefriend (string friendname) {
        auto it = find (friends.begin(),friends.end(),friendname);

        // checking if friend exist
        if (it != friends.end()) {
            friends.erase (it);
        }
        else {
            cout << "freind does not found \n";
        }
    }

    void showfriends () {
        // checking if a person has no freinds.
        if (friends.empty()) {
            cout << "No friends yet.";
            return;
        }     
        cout << "Friends of " << name << ": ";

        for(string &f: friends) {
            cout << f << " ";
        }
        cout << "\n";
    }

    void sendMessage (User &receiver, string message) {
       receiver.inbox.push_back({this->name, message});
       cout << "Message sent from " << this->name << " to " << receiver.getName() << endl;
    }

    void receiveMessage (User &sender, string message) {
        inbox.push_back ({sender.getName(),message});
        cout << "Message receive from " << sender.getName();
    }

    void showInbox () {
        cout << "\nInbox of " << name << ":\n";
        // inbox is empty
        if (inbox.empty()) 
            cout << "No messages yet.\n";
        else {
            for(auto &it : inbox) {
                cout << it.first << " => " << it.second << endl;
            }
        }
    }

    vector<string> getFriends()  {
        return friends;
    }
};

class SocialNetwork {
private:
    unordered_map <string,User> users; 
public:

    bool UserExists (string name) {                // unodered maps provide O(1) searching
        // checking if user exists by name
        if (users.find (name) != users.end()) {
            return true;
        }
        else {
            return false;
        }
    }

    User& getUser (string name) {
        return users[name];
    }

    void addUser (string name, int age) {
        // checking id user already
        if (UserExists(name)) {
            cout << "User already exists.\n";
            return;
        }
        users[name] = User(name, age);
        cout << "User " << name << " added successfully.\n";
    }

    void addConnection (string u, string v) {
        if (!UserExists(u) || !UserExists(v)) {
            cout << "One or both users not found.\n";
            return;
        }
        users[u].addfriend (v);
        users[v].addfriend (u);

        cout << "\nConnection added between users: " << u << " and " << v;
    }

    void showfreinds (string name) {
        if(!UserExists (name)) {
            cout << "\nUser does not exist:  ";
            return;
        }
        else {
            users[name].showfriends();
        }
    }

    bool isConnected (string u, string v) {
        // check using BFS traversal
        queue<string> q;

        unordered_map<string,bool> visited;
        visited[u] = true;
        q.push(u);

        while (!q.empty())
        {
            string current = q.front();
            q.pop();

            if(current == v) return true;

            for (auto &it: users[current].getFriends()) {
                if (!visited[it]) {
                    visited[it] = true;
                    q.push(it);
                }
            }
        }
        return false;
    }

    void DFS_helper (string u , unordered_map<string,bool> &visited) {
        cout << u << " ";
        visited[u] = true;

        for (auto &v: users[u].getFriends()) {
            if (!visited [v]) {
                DFS_helper (v, visited);
            }
        }
    }

    void find_communities() {
        int group = 0;

        unordered_map<string,bool> visited ;
        cout << "\nCommunities:\n";
        for (auto &it: users)
        {
            string name = it.first;
            if (!visited[name]) {
                group++;
                cout << "Group " << group << ": ";
                DFS_helper (name,visited);
            }   
            cout << endl;
        }
        cout << endl;
    }

    void sendMessage (string sender, string receiver, string message) {
      if (!UserExists(sender) || !UserExists(receiver)) {
        cout << "\nUser does not exist.\n";
        return;
    }
        users[receiver].receiveMessage(users[sender], message);
    }

    void showInbox(string username) {
    if (!UserExists(username)) {
        cout << "User not found.\n";
        return;
    }
        users[username].showInbox();
    }

    void showAllUsers() {
        cout << "\nRegistered Users:\n";
        for (auto &p : users) {
            cout << "- " << p.first << " (" << p.second.getAge() << ")\n";
        }       
    }

    bool detectCycleUtil(const string &u, unordered_map<string,bool> &visited, const string &parent) {
        visited[u] = true;

        for (auto &v : users[u].getFriends()) {
            if (!visited[v]) {
                if (detectCycleUtil(v, visited, u))
                    return true;
            }
            else if (v != parent) {
                return true;
            }
        }
        return false;
    }

    bool detectCycle() {
        unordered_map<string,bool> visited;

        for (auto &it : users) {
            if (!visited[it.first]) {
                if (detectCycleUtil(it.first, visited, ""))
                    return true;
                }
        }
        return false;
    }

    void shortestPath(string source, string destination) {
        if (!UserExists(source) || !UserExists(destination)) {
            cout << "One or both users not found.\n";
            return;
        }

        unordered_map<string, bool> visited;
        unordered_map<string, string> parent; // to reconstruct path
        queue<string> q;

        visited[source] = true;
        q.push(source);

        bool found = false;

        while (!q.empty()) {
            string current = q.front();
            q.pop();

            if (current == destination) {
                found = true;
                break;
            }

        for (auto &friendName : users[current].getFriends()) {
            if (!visited[friendName]) {
                visited[friendName] = true;
                parent[friendName] = current;
                q.push(friendName);
            }
        }
    }

    if (!found) {
        cout << "No path exists between " << source << " and " << destination << ".\n";
        return;
    }

    vector<string> path;
    string current = destination;
    while (current != source) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(source);

    reverse(path.begin(), path.end());

    cout << "Shortest path (" << path.size() - 1 << " connections): ";
    for (auto &user : path) {
        cout << user;
        if (user != destination) cout << " -> ";
    }
    cout << endl;
}

     
    vector<string> suggestFriends(string username, int maxSuggestions) {
        vector<string> suggestions;

    if (!UserExists(username)) {
        cout << "User not found.\n";
        return suggestions;
    }

    unordered_set<string> directFriends(users[username].getFriends().begin(),
                                        users[username].getFriends().end());
    unordered_map<string, int> mutualCount; // count mutual friends

    // Check friends of friends
    for (auto &friendName : directFriends) {
        for (auto &fof : users[friendName].getFriends()) {
            // ignore self and existing direct friends
            if (fof != username && directFriends.find(fof) == directFriends.end()) {
                mutualCount[fof]++;
            }
        }
    }

    // Create a vector of suggestions sorted by mutual friends count
    vector<pair<int, string>> sortedSuggestions;
    for (auto &p : mutualCount) {
        sortedSuggestions.push_back({p.second, p.first});
    }

    // Sort in descending order of mutual friends
    sort(sortedSuggestions.begin(), sortedSuggestions.end(), greater<>());

    // Pick top maxSuggestions
    for (int i = 0; i < min(maxSuggestions, (int)sortedSuggestions.size()); i++) {
        suggestions.push_back(sortedSuggestions[i].second);
    }

    return suggestions;
}



    vector<string> commonFriends(string user1, string user2) {
        vector<string> commons;

    if (!UserExists(user1) || !UserExists(user2)) {
        cout << "One or both users not found.\n";
        return commons;
    }

    // Get friends of both users
    unordered_set<string> friends1(users[user1].getFriends().begin(), users[user1].getFriends().end());
    unordered_set<string> friends2(users[user2].getFriends().begin(), users[user2].getFriends().end());

    // Find intersection
    for (const string &f : friends1) {
        if (friends2.find(f) != friends2.end()) {
            commons.push_back(f);
        }
    }

    return commons;
}


    // vector<string> mostActiveUsers() {
    //     // to be implemented later
    // }


    // void removeInactiveUsers() {
    //     // to be implemented later
    // }

    // void ActiveUsers() {
    //     // to be implemented later
    // }

    // vector<string> recommendGroups() {
    //     // to be implemented later
    // }

};


// funcion to check integer
bool isValidUsername(const string &name) {
    return !name.empty() && name.length() <= 20;
}

int getSafeInt() {
    int x;
    while (true) {
        cin >> x;
        if (!cin.fail()) return x;

        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a number: ";
    }
}

string getValidUsername() {
    string name;

    while (true) {
        getline(cin, name);
        if (isValidUsername(name))
            return name;

        cout << "Invalid username! Must be 1-20 characters. Try again: ";
    }
}

int getValidAge() {
    cout << "Enter age (0-120): ";
    int age = getSafeInt();

    while (age < 0 || age > 120) {
        cout << "Invalid age! Enter a value between 0 and 120: ";
        age = getSafeInt();
    }
    return age;
}


int main()
{
    SocialNetwork sn;
    int choice;
    string u, v, msg;
    int age;

    while (true) {
        cout << "\n==== Social Network Simulator ====\n";
        cout << "1.Add User";
        cout << "\n2. Add Connection ";
        cout << "\n3. Show Friends ";
        cout << "\n4. Check Connection ";
        cout << "\n5. Shortest Path ";
        cout << "\n6. Find Communities ";
        cout << "\n7. Send Message ";
        cout << "\n8. Show Inbox ";
        cout << "\n9. Show All Users ";
        cout << "\n10. Exit\n";
       
        cout << "Enter your choice: ";
        choice = getSafeInt();
        cin.ignore();

        if (choice == 1) {
            cout << "Enter username, age: ";
            u = getValidUsername();
            age = getValidAge();
            sn.addUser(u, age);
        }
        else if (choice == 2) {
            cout << "Enter first username: ";
            // cin >> u;
            u = getValidUsername(); 
            cout << "Enter second username: ";
            // cin >> v;
            v = getValidUsername(); 
            sn.addConnection(u, v);
        }
        else if (choice == 3) {
            cout << "Enter username: ";
            u = getValidUsername();
            sn.showfreinds(u);
        }
        else if (choice == 4) {
            cout << "Enter first username: ";
            u = getValidUsername();
            cout << "Enter second username: ";
            v = getValidUsername();
            cout << (sn.isConnected(u, v) ? "Connected\n" : "Not Connected\n");
        }
        else if (choice == 5) {
            cout << "Enter source: ";
            u = getValidUsername();
            cout << "Enter destination ";
            v = getValidUsername();
            sn.shortestPath(u, v);
        }
        else if (choice == 6) {
            sn.find_communities();
        }
        else if (choice == 7) {
            cout << "Enter sender name: ";
            u = getValidUsername();
            cout << "Enter receiver name: ";
            v  = getValidUsername();
            cout << "\nEnter message you want to send: ";
            getline(cin, msg);

            sn.sendMessage(u, v, msg);
        }
        else if (choice == 8) {
            cout << "Enter username: ";
            u = getValidUsername();
            sn.showInbox(u);
        }
        else if (choice == 9) {
            sn.showAllUsers();
        }
        else if (choice == 10) {
            cout << "Exiting...\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}