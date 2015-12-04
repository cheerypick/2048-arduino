package no.nith.pg5100.controller;

import no.nith.pg5100.dto.EventType;
import no.nith.pg5100.dto.Location;
import no.nith.pg5100.dto.Subject;
import no.nith.pg5100.dto.User;
import no.nith.pg5100.infrastructure.location.LocationDao;
import no.nith.pg5100.infrastructure.subject.SubjectDao;
import no.nith.pg5100.infrastructure.user.JpaUser;
import no.nith.pg5100.infrastructure.user.UserDao;

import javax.annotation.PostConstruct;
import javax.enterprise.inject.Model;
import javax.faces.model.SelectItem;
import javax.inject.Inject;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.Stream;

@Model
public class SubjectController {
    @Inject
    private SubjectDao subjectDao;

    @Inject
    private LocationDao locationDao;

    @Inject @JpaUser
    private UserDao userDao;

    private int subjectId;
    private Subject subject;

    private int locationId;
    private List<String> userIds;

    @PostConstruct
    public void init() {
        subject = new Subject();
    }

    public List<Subject> getAll() {
        return subjectDao.getAll();
    }

    public int getSubjectId() {
        return subjectId;
    }

    public void setSubjectId(int subjectId) {
        this.subjectId = subjectId;
    }

    public Subject getSubject() {
        return subject;
    }

    public void setSubject(Subject subject) {
        this.subject = subject;
    }

    public int getLocationId() {
        return locationId;
    }

    public void setLocationId(int locationId) {
        this.locationId = locationId;
    }

    public List<String> getUserIds() {
        return userIds;
    }

    public void setUserIds(List<String> userIds) {
        this.userIds = userIds;
    }

    public void initSubject() {
        subject = subjectDao.findById(subjectId);
    }

    public String getSelectedLocation() {
        Location location = subject.getLocation();
        return location.getBuilding() + " - " + location.getRoom();
    }

    public List<String> getSelectedUsers() {
        List<User> users = subject.getUsers();
        return users.stream().map(User::getEmail).collect(Collectors.toList());
    }

    public List<SelectItem> getLocations() {
        List<Location> locations = locationDao.getAll();
        return locations.stream().map(l -> new SelectItem(l.getId(), l.getBuilding() + " - " + l.getRoom())).collect(Collectors.toList());
    }

    public List<SelectItem> getUsers() {
        List<User> users = userDao.getAll();
        return users.stream().map(u -> new SelectItem(u.getId(), u.getEmail())).collect(Collectors.toList());
    }

    public void persist() {
        Location location = locationDao.findById(locationId);
        subject.setLocation(location);

        List<User> users = userIds.stream().map(id -> userDao.findById(Integer.parseInt(id))).collect(Collectors.toList());
        subject.setUsers(users);

        subjectDao.persist(subject);
    }

    public List<SelectItem> getSubjects() {
        return getAll().stream()
                .map(s -> new SelectItem(s.getId(), s.getName()))
                .collect(Collectors.toList());
    }
}
