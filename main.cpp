#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;

class Course {
public:
    string courseId;
    string name;
    unordered_set<string> prerequisites;

    Course(const string& id, const string& name)
        : courseId(id), name(name) {}
};

/*
 * CourseManager code
 */
class CourseManager {
private:
    unordered_map<string, Course*> courses;

    // DFS to detect the cycles
    bool hasCycle(const string& start,
                  const string& current,
                  unordered_set<string>& visited) {
        if (current == start) return true;
        if (!courses.count(current) || visited.count(current)) return false;

        visited.insert(current);
        for (const auto& prereq : courses[current]->prerequisites) {
            if (hasCycle(start, prereq, visited)) {
                return true;
            }
        }
        return false;
    }

    // DFS to list all prerequisites
    void dfs(const string& courseId,
             unordered_set<string>& visited,
             vector<string>& result) {
        for (const auto& prereq : courses[courseId]->prerequisites) {
            if (!visited.count(prereq)) {
                visited.insert(prereq);
                result.push_back(prereq);
                dfs(prereq, visited, result);
            }
        }
    }

public:
    ~CourseManager() {
        for (auto& pair : courses) {
            delete pair.second;
        }
    }

    void addCourse(const string& courseId, const string& name) {
        if (courses.count(courseId)) {
            throw runtime_error("Course already exists");
        }
        courses[courseId] = new Course(courseId, name);
    }

    void addPrerequisite(const string& courseId, const string& prereqId) {
        if (courseId == prereqId) {
            throw runtime_error("Course cannot be its own prerequisite");
        }
        if (!courses.count(courseId) || !courses.count(prereqId)) {
            throw runtime_error("Course not found");
        }

        courses[courseId]->prerequisites.insert(prereqId);

        unordered_set<string> visited;
        if (hasCycle(courseId, prereqId, visited)) {
            courses[courseId]->prerequisites.erase(prereqId);
            throw runtime_error("Adding this prerequisite creates a cycle");
        }
    }

    void removeCourse(const string& courseId) {
        if (!courses.count(courseId)) return;

        delete courses[courseId];
        courses.erase(courseId);

        for (auto& pair : courses) {
            pair.second->prerequisites.erase(courseId);
        }
    }

    vector<string> listPrerequisites(const string& courseId) {
        if (!courses.count(courseId)) return {};

        unordered_set<string> visited;
        vector<string> result;
        dfs(courseId, visited, result);
        return result;
    }

    bool canEnroll(const string& courseId,
                   const unordered_set<string>& completedCourses) {
        auto prereqs = listPrerequisites(courseId);
        for (const auto& prereq : prereqs) {
            if (!completedCourses.count(prereq)) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    CourseManager manager;

    manager.addCourse("CS101", "Intro to CS");
    manager.addCourse("CS201", "Data Structures");
    manager.addCourse("CS301", "Algorithms");

    manager.addPrerequisite("CS201", "CS101");
    manager.addPrerequisite("CS301", "CS201");

    cout << "Prerequisites for CS301: ";
    auto prereqs = manager.listPrerequisites("CS301");
    for (const auto& p : prereqs) {
        cout << p << " ";
    }
    cout << endl;

    unordered_set<string> completed1 = {"CS101", "CS201"};
    unordered_set<string> completed2 = {"CS101"};

    cout << "Can enroll (CS101, CS201): "
         << manager.canEnroll("CS301", completed1) << endl;

    cout << "Can enroll (CS101): "
         << manager.canEnroll("CS301", completed2) << endl;

    return 0;
}
