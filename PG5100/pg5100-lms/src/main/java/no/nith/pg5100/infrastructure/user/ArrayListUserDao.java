package no.nith.pg5100.infrastructure.user;

import no.nith.pg5100.dto.User;

import java.util.ArrayList;
import java.util.List;

public class ArrayListUserDao implements UserDao {
    private final List<User> users = new ArrayList<>();

    @Override
    public User persist(User user) {
        user.setId(users.size());
        users.add(user);
        return user;
    }

    @Override
    public boolean update(User user) {
        users.set(user.getId(), user);
        return true;
    }

    @Override
    public User findById(int id) {
        return users.get(id);
    }

    @Override
    public List<User> getAll() {
        return users;
    }

    @Override
    public boolean remove(User user) {
        users.set(user.getId(), null);
        return true;
    }
}
