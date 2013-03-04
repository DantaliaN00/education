require 'spec_helper'

describe MoviesController do
  describe 'movies with same directors' do
    it 'should call the model method that performs same directors query' do
      fake_movie = mock('movie1')
      fake_movie.stub(:id).and_return(1)
      fake_movie.stub(:director).and_return('test_director')
      fake_results = [fake_movie]
      Movie.stub(:find).and_return(fake_movie)
      get 'with_same_director', {:id => 1}
      #controller.should_receive(:show).with(:id => 1)
    end
    it 'should select the same directors template for rendering' do
      fake_movie = mock('movie1')
      fake_movie.stub(:id).and_return(1)
      fake_movie.stub(:director).and_return('test_director')
      fake_results = [fake_movie]
      Movie.stub(:find).and_return(fake_movie)
      Movie.stub(:find_all_by_director).and_return(fake_results)
      get 'with_same_director', {:id => 1}
      response.should render_template('show')
    end
    it 'should make the same directors results availabe to that template' do
      fake_movie = mock('movie1')
      fake_movie.stub(:id).and_return(1)
      fake_movie.stub(:director).and_return('test_director')
      fake_results = [fake_movie]
      Movie.stub(:find).and_return(fake_movie)
      Movie.stub(:find_all_by_director).and_return(fake_results)
      get 'with_same_director', {:id => 1}
      assigns(:movies).should == fake_results
    end
  end

  describe 'edit movies controller' do
    it 'should get movie' do
      fake_movie = mock('movie1')
      fake_movie.stub(:id).and_return(1)
      fake_movie.stub(:title).and_return("a")
      fake_movie.stub(:director).and_return("")
      Movie.stub(:find).and_return(fake_movie)
      get :edit, {:id => 1}
      assigns(:movie).should == fake_movie
    end
  end

  describe 'show movies controller' do
    it 'should sort by title' do
      get 'index', {:ratings => "G", :sort => "title"}
    end

    it ' should update a movie' do
      Movie.create(:title => 'test', :id => 1)
      put :update, :id => 1, :movie => {:title=> 'changed'}
    end

    it 'should have diff params then session' do
#      @select_ratings = {'G'}
      get 'index', {}
    end

    it 'should sort by release date' do
      get 'index', {:release_date => Time.now, :sort => "release_date"}
    end

    it 'should create new movie' do
      post :create, {:id => 1}
    end

    it 'should get ' do
      #controller.should_receive(:show)
      #get :with_same_director, {:id => 1}
    end

    it 'should check if no directors' do
      fake_movie = mock('movie1')
      fake_movie.stub(:id).and_return(1)
      fake_movie.stub(:title).and_return("a")
      fake_movie.stub(:director).and_return("")
      fake_results = [fake_movie]
      Movie.stub(:find).and_return(fake_movie)
      Movie.stub(:find_all_by_director).and_return(fake_results)
      
      get :with_same_director, {:id => 1}
      response.should redirect_to(movies_path)
    end

    it 'should destroy a movie' do
      Movie.create(:title => 'test', :director => 'test_dir', :id => 1)
      delete :destroy, {:id => 1}
      Movie.count.should == 0
    end

    it 'controller should show' do
      Movie.create(:title => 'test', :director => 'test_dir', :id => 1)
      get 'show', {:id=>1}
    end

  end
end
