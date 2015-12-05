package no.nith.pg5100.controller;

import no.nith.pg5100.dto.Subject;
import no.nith.pg5100.dto.User;
import no.nith.pg5100.dto.UserType;
import no.nith.pg5100.infrastructure.user.JpaUser;
import no.nith.pg5100.infrastructure.user.UserDao;

import javax.annotation.PostConstruct;
import javax.enterprise.inject.Model;
import javax.faces.model.SelectItem;
import javax.inject.Inject;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

@Model
public class UserController {
    @Inject @JpaUser
    private UserDao userDao;

    private int selectedId;
    private User user;

    @PostConstruct
    public void init() {
        user = new User();
    }

    public void persistNewUser() {
        userDao.persist(user);
    }

    public List<User> getAll() {
        return userDao.getAll();
    }

    public void fetchUser() {
        user = userDao.findById(selectedId);
    }

    public int getSelectedId() {
        return selectedId;
    }

    public void setSelectedId(int selectedId) {
        this.selectedId = selectedId;
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public List<SelectItem> getUserTypes() {
        return Arrays.asList(UserType.values()).stream().map(t -> new SelectItem(t, t.name())).collect(Collectors.toList());
    }

    public List<String> getSelectedSubjects() {
        return user.getSubjects().stream().map(Subject::getName).collect(Collectors.toList());
    }


}
