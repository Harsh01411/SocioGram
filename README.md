# SocioGram
SocioGram Social Media App. Tectonic


This program simulates a basic social network where users can register, log in, follow others, create and like posts, and view profiles and feeds. It’s written in C++ and features simple interaction through a text-based menu.

Features:
1.User Registration & Login: Users can sign up with basic details and log in using their username.
2. Follow & Unfollow: Users can follow or unfollow others to see their posts in the feed.
3. Posts & Likes: Users can create posts (max 500 characters) and like posts.
4. Profile Privacy: Users set their profile to either public or private, with private profiles being visible only to mutual followers.
5. Feed: Displays posts from users you follow, sorted by the most recent.
Classes:
1. Post: Represents a post with content, timestamp, and like count.
2. User: Contains user details, posts, followers, and methods for following, unfollowing, and interacting with posts.
3. SocialNetwork: Manages user registration, login, and interactions like following, posting, and liking.
Example Flow:
1. Register a user with basic info.
2. Log in and follow other users.
3. Create posts and like others' posts.
4. View your feed and check profiles.
