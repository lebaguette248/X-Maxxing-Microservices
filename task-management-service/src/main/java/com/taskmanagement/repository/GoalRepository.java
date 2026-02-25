package com.taskmanagement.repository;

import com.taskmanagement.entity.Goal;
import com.taskmanagement.entity.Goal.GoalStatus;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.time.LocalDate;
import java.util.List;
import java.util.Optional;

@Repository
public interface GoalRepository extends JpaRepository<Goal, Long> {

    List<Goal> findByUserId(Long userId);

    Page<Goal> findByUserId(Long userId, Pageable pageable);

    List<Goal> findByUserIdAndStatus(Long userId, GoalStatus status);

    @Query("SELECT g FROM Goal g LEFT JOIN FETCH g.tasks WHERE g.id = :id")
    Optional<Goal> findByIdWithTasks(@Param("id") Long id);

    @Query("SELECT g FROM Goal g WHERE g.userId = :userId AND g.status != 'COMPLETED' " +
           "AND g.targetDate <= :date ORDER BY g.targetDate ASC")
    List<Goal> findUpcomingDeadlines(@Param("userId") Long userId, @Param("date") LocalDate date);

    @Query("SELECT g FROM Goal g WHERE g.userId = :userId " +
           "AND (:status IS NULL OR g.status = :status) " +
           "AND (:priority IS NULL OR g.priority = :priority)")
    Page<Goal> findByFilters(
        @Param("userId") Long userId,
        @Param("status") GoalStatus status,
        @Param("priority") Goal.Priority priority,
        Pageable pageable
    );

    @Query("SELECT COUNT(g) FROM Goal g WHERE g.userId = :userId AND g.status = :status")
    long countByUserIdAndStatus(@Param("userId") Long userId, @Param("status") GoalStatus status);

    @Query("SELECT AVG(g.progressPercentage) FROM Goal g WHERE g.userId = :userId")
    Double getAverageProgressByUserId(@Param("userId") Long userId);
}
