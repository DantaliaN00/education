class MoviesController < ApplicationController

  def show
    id = params[:id] # retrieve movie ID from URI route
    @movie = Movie.find(id) # look up movie by unique ID
    # will render app/views/movies/show.<extension> by default
  end

  def index
    session[:ratings] = params[:ratings] unless params[:ratings].nil?
    session[:s] = params[:s] unless params[:s].nil?

    if params[:ratings].nil? and params[:s].nil? and (!session[:ratings].nil? or !session[:s].nil?)
      redirect_to movies_path(:ratings => session[:ratings], :s => session[:s] )
    end

    @all_ratings = Movie.ratings
    if session[:ratings]
      ratings = session[:ratings].keys
    else
      ratings = Movie.ratings
    end
 
    if session[:s] == 't' 
        @movies = Movie.find(:all, :order => :title, :conditions => { :rating => ratings })
    elsif session[:s] == 'd' 
        @movies = Movie.find(:all, :order => :release_date, :conditions => { :rating => ratings })
    else
        @movies = Movie.find(:all, :conditions => { :rating => ratings })
    end

  end

  def new
    # default: render 'new' template
  end

  def create
    @movie = Movie.create!(params[:movie])
    flash[:notice] = "#{@movie.title} was successfully created."
    redirect_to movies_path
  end

  def edit
    @movie = Movie.find params[:id]
  end

  def update
    @movie = Movie.find params[:id]
    @movie.update_attributes!(params[:movie])
    flash[:notice] = "#{@movie.title} was successfully updated."
    redirect_to movie_path(@movie)
  end

  def destroy
    @movie = Movie.find(params[:id])
    @movie.destroy
    flash[:notice] = "Movie '#{@movie.title}' deleted."
    redirect_to movies_path
  end

end
