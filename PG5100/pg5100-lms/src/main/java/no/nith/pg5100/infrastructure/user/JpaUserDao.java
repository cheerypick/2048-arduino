package no.nith.pg5100.infrastructure.user;

import no.nith.pg5100.dto.User;

import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.TypedQuery;
import java.util.List;

@JpaUser
@Stateless
public class JpaUserDao implements UserDao {

    @PersistenceContext(unitName = "pg5100-lms")
    private EntityManager entityManager;

    public JpaUserDao() {
    }

    JpaUserDao(EntityManager entityManager) {
        this.entityManager = entityManager;
    }

    @Override
    public User persist(User user) {
        entityManager.persist(user);
        return user;
    }

    @Override
    public boolean update(User user) {
        if (!entityManager.contains(user)) {
            entityManager.merge(user);
        }

        return true;
    }

    @Override
    public User findById(int id) {
        return entityManager.find(User.class, id);
    }

    @Override
    public List<User> getAll() {
        TypedQuery<User> query = entityManager.createNamedQuery("User.getAll", User.class);
        return query.getResultList();
    }

    @Override
    public boolean remove(User user) {
        entityManager.remove(user);
        return true;
    }
}
