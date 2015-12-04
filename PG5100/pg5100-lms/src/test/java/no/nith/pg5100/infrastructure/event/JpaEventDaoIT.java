package no.nith.pg5100.infrastructure.event;

import no.nith.pg5100.dto.Event;
import no.nith.pg5100.dto.EventType;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.Date;
import java.util.List;

import static org.junit.Assert.*;

public class JpaEventDaoIT {
    private EntityManagerFactory factory;
    private EntityManager entityManager;
    private JpaEventDao eventDao;

    @Before
    public void setUp() throws Exception {
        factory = Persistence.createEntityManagerFactory("pg5100-lms");
        entityManager = factory.createEntityManager();
        eventDao = new JpaEventDao(entityManager);
    }

    @After
    public void tearDown() throws Exception {
        entityManager.close();
        factory.close();
    }

    @Test
    public void persist() throws Exception {
        Event event = new Event();
        event.setTitle("Julebord");
        event.setStartDateTime(new Date());
        event.setEventType(EventType.LECTURE);

        entityManager.getTransaction().begin();
        Event result = eventDao.persist(event);
        entityManager.getTransaction().commit();
        System.out.println(result.getId());
        assertTrue(result.getId() > 0);
    }

    @Test
    public void findById() throws Exception {
        Event event = eventDao.findById(1);
        assertEquals("Repetisjonsforelesning", event.getTitle());
        assertEquals(EventType.LECTURE, event.getEventType());
//        assertEquals("PG6100", event.getSubjectId().getName());
//        System.out.println(event.getSubjectId().getName());

    }

    @Test
    public void getAll() throws Exception {
        List<Event> events = eventDao.getAll();
        assertEquals(2, events.size());
    }


}