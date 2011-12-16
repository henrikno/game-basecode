#pragma once
#include <map>

template<typename T>
class ResourceManager {
public:
    typedef std::pair<std::string, T*> Resource;
    typedef std::map<std::string, T*>  ResourceMap;
private:
    ResourceMap myResources;
    T* find(const std::string &id) {
        T* resource = NULL;
        typename ResourceMap::iterator it = myResources.find(id);
        if (it != myResources.end()) {
            resource = it->second;
        }
        return resource;
    }

protected:
    virtual T* load(const std::string &id) = 0;

public:
    ResourceManager()
      : myResources()
    {
    }
    virtual ~ResourceManager() {
        ReleaseAllResources();
    }
    T* GetResource(const std::string &id) {
        T* resource = find(id);
        if (resource == NULL) {
            resource = load(id);
            if (resource != NULL)
                myResources.insert(Resource(id, resource));
        }
        return resource;
    }

    void ReleaseResource(const std::string &id) {
        T* resource = find(id);
        if (resource != NULL) {
            delete resource;
            myResources.erase(myResources.find(id));
        }
    }

    void ReleaseAllResources() {
        while (myResources.begin() != myResources.end()) {
            delete myResources.begin()->second;
            myResources.erase(myResources.begin());
        }
    }
};
