package no.nith.pg5100.infrastructure.subject;

import no.nith.pg5100.dto.Subject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

import java.util.ArrayList;
import java.util.List;

import static org.junit.Assert.*;

public class JpaSubjectDaoIT {
    private EntityManagerFactory factory;
    private EntityManager entityManager;
    private JpaSubjectDao subjectDao;

    @Before
    public void setUp() throws Exception {
        factory = Persistence.createEntityManagerFactory("pg5100-lms");
        entityManager = factory.createEntityManager();
        subjectDao = new JpaSubjectDao(entityManager);
    }

    @After
    public void tearDown() throws Exception {
        entityManager.close();
        factory.close();
    }

    @Test
    public void persist() throws Exception {
        Subject subject = new Subject();
        subject.setName("PG5200");
        subject.setUsers(new ArrayList<>());

        entityManager.getTransaction().begin();
        Subject result = subjectDao.persist(subject);
        entityManager.getTransaction().commit();

        assertTrue(result.getId() > 0);
    }

    @Test
    public void findById() throws Exception {
        Subject subject = subjectDao.findById(1);
        assertEquals("PG5100", subject.getName());
    }

    @Test
    public void getAll() throws Exception {
        List<Subject> subjects = subjectDao.getAll();
        assertEquals(2, subjects.size());
    }
}