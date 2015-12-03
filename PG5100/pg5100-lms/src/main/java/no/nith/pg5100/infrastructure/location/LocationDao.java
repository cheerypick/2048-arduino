package no.nith.pg5100.infrastructure.location;

import no.nith.pg5100.dto.Location;

import java.util.List;

public interface LocationDao {
    Location persist(Location location);
    Location findById(int id);
    List<Location> getAll();
}
