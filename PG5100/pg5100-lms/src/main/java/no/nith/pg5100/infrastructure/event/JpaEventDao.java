package no.nith.pg5100.infrastructure.event;

import no.nith.pg5100.dto.Event;

import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.TypedQuery;
import java.util.List;

@Stateless
public class JpaEventDao implements EventDao {

    @PersistenceContext(unitName = "pg5100-lms")
    private EntityManager entityManager;

    public JpaEventDao() {
    }

    JpaEventDao(EntityManager entityManager) {
        this.entityManager = entityManager;
    }

    @Override
    public Event persist(Event event) {
        entityManager.persist(event);
        return event;
    }

    @Override
    public Event findById(int id) {
        return entityManager.find(Event.class, id);
    }

    @Override
    public List<Event> getAll() {
        TypedQuery<Event> query = entityManager.createNamedQuery("Event.getAll", Event.class);
        return query.getResultList();
    }
}
