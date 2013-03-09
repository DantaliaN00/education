Feature: Merge Articles
  As a blog administrator
  In Order to remove similar articles 
  I want to be able to merge two article into one

  Background:
    Given the blog is set up
    And I am logged into the admin panel

  Scenario: Successfully merge two articles
    Given I am on the new article page
    When I fill in "article_title" with "Article1"
    And I fill in "article__body_and_extended_editor" with "Lorem Ipsum1"
    And I press "Publish"
    Then I should be on the admin content page
    When I go to the home page
    Then I should see "Article1"
    When I follow "Article1"
    Then I should see "Lorem Ipsum1"
    
    Given I am on the new article page
    When I fill in "article_title" with "Article2"
    And I fill in "article__body_and_extended_editor" with "Lorem Ipsum2"
    And I press "Publish"
    Then I should be on the admin content page
    When I go to the home page
    Then I should see "Article2"
    When I follow "Article2"
    Then I should see "Lorem Ipsum2"

    Given I am on the admin content page
    And I follow "Article1"
    And I fill in "merge_with" with "4"
    And I press "Merge"
    
    When I go to the home page
    Then I should see "Article1"
    When I follow "Article1"
    Then I should see "Lorem Ipsum1"
    And I should see "Lorem Ipsum2"
        






