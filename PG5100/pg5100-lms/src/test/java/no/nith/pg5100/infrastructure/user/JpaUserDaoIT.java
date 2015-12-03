package no.nith.pg5100.infrastructure.user;

import no.nith.pg5100.dto.User;
import no.nith.pg5100.dto.UserType;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.List;

import static org.junit.Assert.*;

public class JpaUserDaoIT {
    private EntityManagerFactory factory;
    private EntityManager entityManager;
    private JpaUserDao userDao;

    @Before
    public void setUp() throws Exception {
        factory = Persistence.createEntityManagerFactory("pg5100-lms");
        entityManager = factory.createEntityManager();
        userDao = new JpaUserDao(entityManager);
    }

    @After
    public void tearDown() throws Exception {
        entityManager.close();
        factory.close();
    }

    @Test
    public void persist() throws Exception {
        User user = new User();
        user.setEmail("per@test.com");
        user.setPassword("abC123");
        user.setUserType(UserType.STUDENT);

        entityManager.getTransaction().begin();
        User result = userDao.persist(user);
        entityManager.getTransaction().commit();
        System.out.println(result.getId());
        assertTrue(result.getId() > 0);
    }

    @Test
    public void findById() throws Exception {
        User user = userDao.findById(1);
        assertEquals("mari@test.com", user.getEmail());
        assertEquals(UserType.STUDENT, user.getUserType());
    }

    @Test
    public void getAll() throws Exception {
        List<User> users = userDao.getAll();
        assertEquals(4, users.size());
    }

    @Test
    public void findByIdAndUpdate() throws Exception {
        User user = userDao.findById(1);
        user.setEmail("test@test.com");

        entityManager.getTransaction().begin();
        boolean updated = userDao.update(user);
        entityManager.getTransaction().commit();

        User result = userDao.findById(1);
        assertTrue(updated);
        assertEquals("test@test.com", result.getEmail());
    }

    @Test
    public void updateDetached() throws Exception {
        User user = userDao.findById(1);
        entityManager.detach(user);

        user.setEmail("test@test.com");
        entityManager.getTransaction().begin();
        boolean updated = userDao.update(user);
        entityManager.getTransaction().commit();

        User result = userDao.findById(1);
        assertTrue(updated);
        assertEquals("test@test.com", result.getEmail());
    }

    @Test
    public void remove() throws Exception {
        User user = userDao.findById(4);

        entityManager.getTransaction().begin();
        boolean removed = userDao.remove(user);
        entityManager.getTransaction().commit();

        User result = userDao.findById(4);
        assertTrue(removed);
        assertNull(result);
    }
}