package no.nith.pg5100.infrastructure.user;

import no.nith.pg5100.dto.User;

import java.util.List;

public interface UserDao {
    User persist(User user);
    boolean update(User user);
    User findById(int id);
    List<User> getAll();
    boolean remove(User user);
}
