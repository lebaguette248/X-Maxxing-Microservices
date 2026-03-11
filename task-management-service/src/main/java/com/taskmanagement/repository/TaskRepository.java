package com.taskmanagement.repository;

import com.taskmanagement.entity.Task;
import com.taskmanagement.entity.Task.TaskStatus;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.time.LocalDate;
import java.util.List;

@Repository
public interface TaskRepository extends JpaRepository<Task, Long> {

    List<Task> findByGoalId(Long goalId);

    List<Task> findByGoalIdOrderByDisplayOrderAsc(Long goalId);

    Page<Task> findByGoalId(Long goalId, Pageable pageable);

    List<Task> findByGoalIdAndStatus(Long goalId, TaskStatus status);

    List<Task> findByGoalIdAndCompleted(Long goalId, boolean completed);

    @Query("SELECT t FROM Task t WHERE t.goal.userId = :userId")
    List<Task> findAllByUserId(@Param("userId") Long userId);

    @Query("SELECT t FROM Task t WHERE t.goal.userId = :userId AND t.completed = false " +
           "AND t.dueDate < :date ORDER BY t.dueDate ASC")
    List<Task> findOverdueTasks(@Param("userId") Long userId, @Param("date") LocalDate date);

    @Query("SELECT t FROM Task t WHERE t.goal.userId = :userId AND t.dueDate = :date")
    List<Task> findTasksDueOn(@Param("userId") Long userId, @Param("date") LocalDate date);

    @Query("SELECT COUNT(t) FROM Task t WHERE t.goal.id = :goalId AND t.completed = true")
    long countCompletedByGoalId(@Param("goalId") Long goalId);

    @Query("SELECT COUNT(t) FROM Task t WHERE t.goal.id = :goalId")
    long countByGoalId(@Param("goalId") Long goalId);

    @Modifying
    @Query("UPDATE Task t SET t.displayOrder = :order WHERE t.id = :id")
    void updateDisplayOrder(@Param("id") Long id, @Param("order") Integer order);

    @Modifying
    @Query("DELETE FROM Task t WHERE t.goal.id = :goalId")
    void deleteAllByGoalId(@Param("goalId") Long goalId);
}
