#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

// Class to represent a Post
class Post {
public:
    string content;
    string user;
    time_t timestamp;
    int likes;

    Post(string content, string user) {
        this->content = content;
        this->user = user;
        this->timestamp = time(0);  // Current time
        this->likes = 0;
    }

    void like() {
        likes++;
    }

    void displayPost() const {
        cout << user << " (" << asctime(localtime(&timestamp)) << "): " << content << " [Likes: " << likes << "]\n";
    }
};

// Class to represent a User
class User {
public:
    string username;
    string name;
    string email;
    string phone;
    string dob;
    string profile_privacy;  // public or private
    vector<Post> posts;
    unordered_map<string, User*> following;
    unordered_map<string, User*> followers;

    User(string username, string name, string email, string phone, string dob, string profile_privacy = "public") {
        this->username = username;
        this->name = name;
        this->email = email;
        this->phone = phone;
        this->dob = dob;
        this->profile_privacy = profile_privacy;
    }

    void follow(User* user) {
        following[user->username] = user;
        user->followers[this->username] = this;
    }

    void unfollow(User* user) {
        following.erase(user->username);
        user->followers.erase(this->username);
    }

    void postMessage(string content) {
        if (content.length() <= 500) {
            posts.push_back(Post(content, username));
        } else {
            cout << "Post exceeds 500 characters limit.\n";
        }
    }

    void displayProfile(User* viewer) {
        if (profile_privacy == "public" || (following.find(viewer->username) != following.end() && viewer->following.find(username) != viewer->following.end())) {
            cout << "Name: " << name << "\nEmail: " << email << "\nPhone: " << phone << "\nDOB: " << dob << "\n";
        } else {
            cout << "Profile is private and not visible to you.\n";
        }
    }

    vector<Post> getFeed() {
        vector<Post> feed;
        for (auto& followee : following) {
            for (const Post& post : followee.second->posts) {
                feed.push_back(post);
            }
        }
        sort(feed.begin(), feed.end(), [](const Post& a, const Post& b) {
            return a.timestamp > b.timestamp;
        });
        return feed;
    }

    void displayPosts() {
        for (int i = 0; i < posts.size(); i++) {
            cout << i + 1 << ". ";
            posts[i].displayPost();
        }
    }
};

// Class to manage the entire Social Network
class SocialNetwork {
private:
    unordered_map<string, User*> users;
    User* loggedInUser;

public:
    SocialNetwork() : loggedInUser(nullptr) {}

    ~SocialNetwork() {
        // Free allocated memory for users
        for (auto& pair : users) {
            delete pair.second;
        }
    }

    void registerUser(string username, string name, string email, string phone, string dob, string profile_privacy = "public") {
        if (users.find(username) != users.end()) {
            cout << "Username already exists.\n";
            return;
        }
        users[username] = new User(username, name, email, phone, dob, profile_privacy);
        cout << "User " << username << " registered successfully.\n";
    }

    void login(string username) {
        if (users.find(username) == users.end()) {
            cout << "User not found.\n";
            return;
        }
        loggedInUser = users[username];
        cout << "Welcome back, " << loggedInUser->username << "!\n";
    }

    void logout() {
        if (loggedInUser != nullptr) {
            cout << "Logged out successfully.\n";
            loggedInUser = nullptr;
        } else {
            cout << "No user is logged in.\n";
        }
    }

    void followUser(string username) {
        if (loggedInUser == nullptr) {
            cout << "Please log in first.\n";
            return;
        }
        if (users.find(username) == users.end()) {
            cout << "User not found.\n";
            return;
        }
        loggedInUser->follow(users[username]);
        cout << loggedInUser->username << " followed " << username << ".\n";
    }

    void unfollowUser(string username) {
        if (loggedInUser == nullptr) {
            cout << "Please log in first.\n";
            return;
        }
        if (users.find(username) == users.end()) {
            cout << "User not found.\n";
            return;
        }
        loggedInUser->unfollow(users[username]);
        cout << loggedInUser->username << " unfollowed " << username << ".\n";
    }

    void createPost(string content) {
        if (loggedInUser == nullptr) {
            cout << "Please log in first.\n";
            return;
        }
        loggedInUser->postMessage(content);
        cout << "Post created successfully.\n";
    }

    void likePost(int postIndex) {
        if (loggedInUser == nullptr) {
            cout << "Please log in first.\n";
            return;
        }
        if (postIndex < 1 || postIndex > loggedInUser->posts.size()) {
            cout << "Invalid post index.\n";
            return;
        }
        loggedInUser->posts[postIndex - 1].like();
        cout << "Post liked successfully.\n";
    }

    void viewProfile(string username) {
        if (loggedInUser == nullptr) {
            cout << "Please log in first.\n";
            return;
        }
        if (users.find(username) == users.end()) {
            cout << "User not found.\n";
            return;
        }
        users[username]->displayProfile(loggedInUser);
    }

    void displayFeed(int page) {
        if (loggedInUser == nullptr) {
            cout << "Please log in first.\n";
            return;
        }

        vector<Post> feed = loggedInUser->getFeed();
        int start = (page - 1) * 20;
        int end = min(start + 20, (int)feed.size());

        if (start >= feed.size()) {
            cout << "No more posts to display.\n";
            return;
        }

        for (int i = start; i < end; i++) {
            feed[i].displayPost();
        }
    }

    User* getLoggedInUser() {
        return loggedInUser;
    }
};

// Main driver program to interact with the user
int main() {
    SocialNetwork network;
    int choice;

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Logout\n4. Follow User\n5. Unfollow User\n6. Create Post\n7. Like Post\n8. View Profile\n9. Display Feed\n10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();  // Ignore newline character after integer input

        switch (choice) {
            case 1: {
                string username, name, email, phone, dob, profile_privacy;
                cout << "Enter username: ";
                getline(cin, username);
                cout << "Enter name: ";
                getline(cin, name);
                cout << "Enter email: ";
                getline(cin, email);
                cout << "Enter phone: ";
                getline(cin, phone);
                cout << "Enter date of birth: ";
                getline(cin, dob);
                cout << "Enter profile privacy (public/private): ";
                getline(cin, profile_privacy);
                network.registerUser(username, name, email, phone, dob, profile_privacy);
                break;
            }
            case 2: {
                string username;
                cout << "Enter username to login: ";
                getline(cin, username);
                network.login(username);
                break;
            }
            case 3:
                network.logout();
                break;
            case 4: {
                string username;
                cout << "Enter username to follow: ";
                getline(cin, username);
                network.followUser(username);
                break;
            }
            case 5: {
                string username;
                cout << "Enter username to unfollow: ";
                getline(cin, username);
                network.unfollowUser(username);
                break;
            }
            case 6: {
                string content;
                cout << "Enter post content (max 500 characters): ";
                getline(cin, content);
                network.createPost(content);
                break;
            }
            case 7: {
                if (network.getLoggedInUser() == nullptr) {
                    cout << "Please log in first.\n";
                    break;
                }
                network.getLoggedInUser()->displayPosts();
                int postIndex;
                cout << "Enter post index to like: ";
                cin >> postIndex;
                network.likePost(postIndex);
                break;
            }
            case 8: {
                string username;
                cout << "Enter username to view profile: ";
                getline(cin, username);
                network.viewProfile(username);
                break;
            }
            case 9: {
                int page;
                cout << "Enter page number to view feed: ";
                cin >> page;
                network.displayFeed(page);
                break;
            }
            case 10:
                return 0;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}
